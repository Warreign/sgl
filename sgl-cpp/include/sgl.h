//---------------------------------------------------------------------------
// sgl.h
// Header file for SGL (Simple Graphics Library)
// Date:  2016/10/24
// Author: Jaroslav Sloup, CTU Prague
// Author: Jaroslav Krivanek, Jiri Bittner, CTU Prague
// Edited: Jakub Hendrich, Daniel Meister, CTU Prague
//
// Implementation notes:
//
// - All functions should set the value of an internal error code variable,
//   whose value can be accessed from the application via sglGetError() and
//   sglGetErrorString() functions.
//
// - All coordinates passed to the library are subject to the
//   modeling/projection/viewport transform.
//
// - Points are column vectors and transformation matrices are 4x4 matrices
//   stored as in OpenGL, that is [column-by-column]. Transformed vector is
//   computed as v_t = M*v.
//
// - The color buffer is a contiguous block of memory which holds scanlines
//   of individual pixels one after another. The pixels themselves are R-G-B
//   triplets with each component stored as a float value.
//
// - In sglEllipse() and sglArc(), you may use adaptive subdivision up to a
//   1 pixel tolerance (in screen space) for a bonus of 1 point.
//
// - No memory leaking is acceptable and will be checked.
//---------------------------------------------------------------------------

#ifndef _SGL_H_
#define _SGL_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Data types
//---------------------------------------------------------------------------

/// Error codes
typedef enum {
  /// Operation succeeded
  SGL_NO_ERROR = 0,
  /// Invalid argument(s) to a call
  SGL_INVALID_VALUE,
  /// Invalid enum argument(s) to a call
  SGL_INVALID_ENUM,
  /// Invalid call (most typical: sglBegin after another sglBegin,
  /// state change inside sglBegin/sglEnd pair, no context defined)
  SGL_INVALID_OPERATION,
  /// Quota of internal resources exceeded (e.g. number of drawing contexts)
  SGL_OUT_OF_RESOURCES,
  /// Internal library error
  SGL_INTERNAL_ERROR,
  /// Matrix stack overflow. Generated by sglPushMatrix().
  SGL_STACK_OVERFLOW,
  /// Matrix stack underflow - trying to pop matrix when there is only one matrix left on the stack.
  /// Generated by sglPopMatrix().
  SGL_STACK_UNDERFLOW,
  /// Insufficient memory to finish the requested operation
  SGL_OUT_OF_MEMORY
} sglEErrorCode;

/// Types of graphic elements which are specified using the sglVertexXf command
typedef enum {
  /// Points
  SGL_POINTS = 1,
  /// Lines
  SGL_LINES,
  /// Line strip
  SGL_LINE_STRIP,
  /// Closed line strip
  SGL_LINE_LOOP,
  /// Triangle list
  SGL_TRIANGLES,
  /// General, non-convex polygon
  SGL_POLYGON,
  /// Area light - restricted to a quadrangle for simplicity
  SGL_AREA_LIGHT,
  SGL_LAST_ELEMENT_TYPE
} sglEElementType;

/// Bits indicating what to clear with sglClear()
typedef enum {
  /// Clear the color buffer
  SGL_COLOR_BUFFER_BIT = 0x0001,
  /// Clear the depth buffer
  SGL_DEPTH_BUFFER_BIT = 0x0002
} sglEClearBit;

/// Enum for selecting matrix stacks to work with. Passed to sglMatrixMode().
typedef enum {
  /// Modelview matrix
  SGL_MODELVIEW = 0,
  /// Projection matrix
  SGL_PROJECTION = 1
} sglEMatrixMode;

/// Enum for area fill modes. Passed to sglAreaMode().
typedef enum {
  /// Draw only vertices
  SGL_POINT = 0,
  /// Draw only borders of graphics elements (lines)
  SGL_LINE = 1,
  /// Draw filled elements, default.
  SGL_FILL = 2
} sglEAreaMode;

/// Enum for sglEnable() / sglDisable()
typedef enum {
  /// enable/disable depth test
  SGL_DEPTH_TEST = 1
} sglEEnableFlags;

//---------------------------------------------------------------------------
// Error handling functions
//---------------------------------------------------------------------------

/// Current error code.
/**
  Returns the current error code stored as an error flag variable. Each
  detectable error is assigned a numeric code and a symbolic name.

  When an error occurs, the error flag is set to the appropriate error code
  value. No other errors are recorded until sglGetError() is called, the error
  code is returned and the flag is reset to SGL_NO_ERROR.

  If a call to sglGetError() returns SGL_NO_ERROR, there has been no detectable
  error since the last call to sglGetError() or since the SGL was initialized.

  ERRORS:
   - none
 */
sglEErrorCode sglGetError(void);

/// Error code string representation.
/**
  Returns the error code string representation which is a read only instance
  belonging to SGL.

  @param error [in] Error code to be interpreted.

  ERRORS:
   - none
 */
const char *sglGetErrorString(sglEErrorCode error);

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

/// Library initialization.
/**
  Initializes the SGL and allocates the internal data structures.

  ERRORS:
   - SGL_OUT_OF_MEMORY
    Not enough memory.
*/
void sglInit(void);

/// Library finalization.
/**
  Finalizes the SGL and disposes the internal data structures.

  ERRORS:
   - none
*/
void sglFinish(void);

/// Drawing context creation.
/**
  Creates a new context for a drawing window with dimensions [width x height]
  of RGB (float, float, float) pixels. Allocates and initializes additional
  internal structures for the context (including the depth buffer,
  transformations set to identities).

  @param width [in] desired canvas width
  @param height [in] desired canvas height
  @return unique identifier of the drawing context

  ERRORS:
   - SGL_OUT_OF_MEMORY
    Not enough memory.
   - SGL_OUT_OF_RESOURCES
    Too many contexts (implementation dependent; at least 32 must be provided,
    though).
*/
int sglCreateContext(int width, int height);

/// Drawing context destruction.
/**
  Destroys the context along with its' internal structures.

  @param id [in] identifier of the context to be destroyed

  ERRORS:
   - SGL_INVALID_VALUE
    Invalid context id (such context doesn't exist).
   - SGL_INVALID_OPERATION
    Context is currently in use.
*/
void sglDestroyContext(int id);

/// Current drawing context selection.
/**
  Selects the current context for subsequent drawing operations.

  @param id [in] identifier of the context to be selected

  ERRORS:
   - SGL_INVALID_VALUE
    Invalid context id.
*/
void sglSetContext(int id);

/// Identifier of the current drawing context.
/**
  Returns the identifier of the current context.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet.
*/
int sglGetContext(void);

/// Address of the current drawing context color buffer.
/**
  Returns the pointer to the color buffer of the current context or NULL if no
  context has been allocated yet (no error code set).

  ERRORS:
   - none
*/
float *sglGetColorBufferPointer(void);

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

/// Clearing a buffer.
/**
  Clears a buffer according to the specified bitmask.

  @param what [in] bitmask consisting of the following bits:
    - SGL_COLOR_BUFFER_BIT ... clear the color buffer (use the color specified by sglClearColor())
    - SGL_DEPTH_BUFFER_BIT ... clear the depth buffer (use a very large positive value)

  ERRORS:
   - SGL_INVALID_VALUE
    Any bit other than those defined is set in the bitmask.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglClear() is called between a call
    to sglBegin() and the corresponding call to sglEnd().
 */
void sglClear(unsigned what);

/// Starting drawing an element.
/**
  Denotes the start of a graphic element specification. The sglBegin() / sglEnd()
  context is essentially a means of input of variable length sequences, that is,
  calls to sglVertexNf(). Other SGL calls are illegal within this context.

  @param mode [in] the element type

  ERRORS:
   - SGL_INVALID_ENUM
    mode is set to an unacceptable value.
   - SGL_INVALID_OPERATION
    sglBegin() is called within another sglBegin() / sglEnd() sequence.
 */
void sglBegin(sglEElementType mode);

/// Ending drawing an element.
/**
  Denotes the end of a graphic element specification.

  ERRORS:
   - SGL_INVALID_OPERATION
    sglEnd() is called with no corresponding previous sglBegin() call.
 */
void sglEnd(void);

/// Input of a 4-D vertex.
/**
  Accepts a four-dimensional vector to be processed with respect to the current
  sglBegin() / sglEnd() sequence type. The effect of invocation outside
  of a sglBegin() / sglEnd() pair is undefined.

  ERRORS:
   - none
 */
void sglVertex4f(float x, float y, float z, float w);

/// Input of a 3-D vertex.
/**
  Accepts a three-dimensional vector (assuming w = 1) to be processed with
  respect to the current sglBegin() / sglEnd() sequence type. The effect of
  invocation outside of a sglBegin() / sglEnd() pair is undefined.

  ERRORS:
   - none
 */
void sglVertex3f(float x, float y, float z);

/// Input of a 2-D vertex.
/**
  Accepts a three-dimensional vector (assuming z = 0, w = 1) to be processed
  with respect to the current sglBegin() / sglEnd() sequence type. The effect of
  invocation outside of a sglBegin() / sglEnd() pair is undefined.

  ERRORS:
   - none
 */
void sglVertex2f(float x, float y);

/// Drawing a circle.
/**
  Draws a circle to the current context color buffer.

  Current transformation is applied _only_ to the coordinates of the center.

  Radius is scaled by the overall scale [x,y] of the concatenated (modelview o
  projection o viewport) transformation. This scaling factor is computed as the
  square root of the determinant of the 2x2 upper left block of the concatenated
  transformation matrix.

  Always draws a circle, no matter the possible non-uniform scale or squash in
  the current transform matrices. In addition, the depth of all the generated
  fragments is constant and equal to the depth of the transformed circle center.

  The fill mode is given by the last call to sglAreaMode(). If SGL_POINT is
  specified, a point in the circle center is drawn.

  @param x [in] circle center x coordinate
  @param y [in] circle center y coordinate
  @param z [in] circle center z coordinate (depth)
  @param radius [in] circle radius

  ERRORS:
   - SGL_INVALID_VALUE
    The radius is not positive.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglCircle() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglCircle(float x, float y, float z, float radius);

/// Drawing an ellipse.
/**
  Draws an ellipse to the current context color buffer. Ellipse axes are aligned
  with coordinate axes.

  Current transformation is applied to the whole ellipse.

  The simplest way to achieve this is to approximate the ellipse with a polyline
  / polygon (depending on the fill mode) and transform each vertex separately.
  Use approximation by a polyline / polygon of exactly 40 vertices (so that the
  speed measurements are fair).

  The fill mode is given by the last call to sglAreaMode(). If SGL_POINT is
  specified, a point in the ellipse center is drawn.

  @param x [in] ellipse center x coordinate
  @param y [in] ellipse center y coordinate
  @param z [in] ellipse center z coordinate (depth)
  @param a [in] x semiaxis length
  @param b [in] y semiaxis length

  ERRORS:
   - SGL_INVALID_VALUE
     The a or b semiaxis length is not positive.
   - SGL_INVALID_OPERATION
     No context has been allocated yet or sglCircle() is called within a
     sglBegin() / sglEnd() sequence.
*/
void sglEllipse(float x, float y, float z, float a, float b);

/// Drawing a circular arc.
/**
  Draws a circular arc to the current context color buffer.

  Current transformation is applied to the whole arc.

  The depth of all the generated fragments is constant and equal to the depth
  of the transformed circle center. Use approximation by a linestrip / polygon
  of exactly 40 * |to - from| / (2 * PI) vertices (so that the speed
  measurements are fair).

  @param x [in] circle center x coordinate
  @param y [in] circle center y coordinate
  @param z [in] circle center z coordinate
  @param radius [in] circle radius
  @param from [in] starting angle in radians (measured counterclockwise from the
                   positive x semiaxis)
  @param to [in] ending angle in radians (measured counterclockwise from the
                 positive x semiaxis)

   ERRORS:
    - SGL_INVALID_VALUE
     The radius is not positive.
    - SGL_INVALID_OPERATION
     No context has been allocated yet or sglArc() is called within a
     sglBegin() / sglEnd() sequence.
 */
void sglArc(float x, float y, float z, float radius, float from, float to);

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

/// Matrix stack selection.
/**
  Specifies which matrix stack is the target for subsequent matrix operations.

  @param mode [in] matrix stack identification: SGL_MODELVIEW or SGL_PROJECTION

  ERRORS:
   - SGL_INVALID_ENUM
    mode is not an accepted value.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMatrixMode() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglMatrixMode(sglEMatrixMode mode);

/// Matrix storage on a stack.
/**
  Duplicates the current transform matrix on top of the current stack.

  ERRORS:
   - SGL_STACK_OVERFLOW
    The current matrix stack capacity has been exhausted.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPushMatrix() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglPushMatrix(void);

/// Matrix disposal from a stack.
/**
  Discards the matrix from top of the current matrix stack.

  ERRORS:
   - SGL_STACK_UNDERFLOW
    The current matrix stack contains only a single matrix.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPopMatrix() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglPopMatrix(void);

/// Identity matrix creation.
/**
  Sets the current transformation matrix to identity.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglLoadIdentity() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglLoadIdentity(void);

/// Arbitrary matrix creation.
/**
  Replaces the current matrix by the given matrix.

  @param matrix [in] arbitrary matrix (elements stored column-by-column)

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglLoadMatrix() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglLoadMatrix(const float *matrix);

/// Matrix multiplication.
/**
  Multiplies the current matrix by the given matrix.

  The multiplication is done so that the transformation represented by the
  given matrix is applied BEFORE the transformation represented by the
  current transform matrix. The operation applied is C := C*M where C is the
  current transformation matrix and M is the matrix passed as the parameter
  (standard behavior in OpenGL).

  @param matrix [in] arbitrary matrix (elements stored column-by-column)

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMultMatrix() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglMultMatrix(const float *matrix);

/// Translation matrix application.
/**
  Multiplies the current matrix by a translation matrix.

  @param x [in] the x coordinate of the translation vector
  @param y [in] the y coordinate of the translation vector
  @param z [in] the z coordinate of the translation vector

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglTranslate() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglTranslate(float x, float y, float z);

/// Scaling matrix application.
/**
  Multiplies the current matrix by a (non-uniform) scale change matrix.

  @param scalex [in] the scaling factor for the x axis
  @param scaley [in] the scaling factor for the y axis
  @param scalez [in] the scaling factor for the z axis

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglScale() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglScale(float scalex, float scaley, float scalez);

/// Rotation matrix application.
/**
  Multiplies the current matrix by a matrix of rotation within the xy plane by
  the given angle around a given center of rotation.

  @param angle [in] the rotation angle in radians measured counterclockwise from
                    the positive x semiaxis
  @param centerx [in] the rotation center x coordinate
  @param centery [in] the rotation center y coordinate

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRotate2D() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglRotate2D(float angle, float centerx, float centery);

/// Rotation matrix application.
/**
  Multiplies the current matrix by a matrix of rotation by the given angle
  around the Y axis.

  @param angle [in] the rotation angle in radians measured counterclockwise from
                    the positive x semiaxis

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRotateY() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglRotateY(float angle);

/// Orthographic matrix application.
/**
  Multiplies the current matrix by an orthographic matrix. See the OpenGL
  reference manual (the blue book) for the exact form of the orthographic matrix.

  @param left [in] the left vertical clipping plane coordinate
  @param right [in] the right vertical clipping plane coordinate
  @param bottom [in] the bottom horizontal clipping plane coordinate
  @param top [in] the top horizontal clipping plane coordinate
  @param near [in] the near depth clipping plane coordinate (negative if the
                   plane is to be behind the viewer)
  @param far [in] the far depth clipping plane coordinate (negative if the
                  plane is to be behind the viewer)

  ERRORS:
   - SGL_INVALID_VALUE
    left = right or bottom = top or near = far.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglOrtho() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglOrtho(float left, float right, float bottom, float top, float near, float far);

/// Perspective matrix application.
/**
  Multiplies the current matrix by a perspective matrix. See the OpenGL
  reference manual (the blue book) for the exact form of the perspective matrix.

  @param left [in] the left vertical clipping plane coordinate
  @param right [in] the right vertical clipping plane coordinate
  @param bottom [in] the bottom horizontal clipping plane coordinate
  @param top [in] the top horizontal clipping plane coordinate
  @param near [in] the near depth clipping plane coordinate (must be positive)
  @param far [in] the far depth clipping plane coordinate (must be positive)

  ERRORS
   - SGL_INVALID_VALUE
    near or far is not positive or left = right or bottom = top or near = far.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglFrustum() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglFrustum(float left, float right, float bottom, float top, float near, float far);

/// Viewport transformation application.
/**
  Specifies the affine transformation from normalized device coordinates to
  window (canvas) coordinates.

  Let (x_nd, y_nd) be normalized device coordinates (computed by transforming
  the vertices by [modelview o projection] matrix). Then the window coordinates
  (x_w, y_w) are computed as follows:

  x_w = (x_nd + 1) * width/2  + x
  y_w = (y_nd + 1) * height/2 + y

  @param x [in] viewport origin x coordinate (with respect to the canvas)
  @param y [in] viewport origin y coordinate (with respect to the canvas)
  @param width [in] viewport width in pixels
  @param height [in] viewport height in pixels

  ERRORS:
   - SGL_INVALID_VALUE
    width or height is not positive.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglViewport() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglViewport(int x, int y, int width, int height);

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

/// Clearing color specification.
/**
  Sets the color for clearing the color buffer. Alpha is ignored.

  @param r [in] the red color component
  @param g [in] the green color component
  @param b [in] the blue color component
  @param alpha [in] the alpha value (ignored, just for conformance with OpenGL)

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglClearColor() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglClearColor(float r, float g, float b, float alpha);

/// Drawing color specification.
/**
  Sets the current drawing color in RGB model for subsequent operations.

  @param r [in] the red color component
  @param g [in] the green color component
  @param b [in] the blue color component

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglClearColor() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglColor3f(float r, float g, float b);

/// Area drawing mode specification.
/**
  Sets the current drawing mode of closed areas for subsequent operations.

  @param mode [in] mode identification:
                   - SGL_POINT: draw only vertices (or center for sglCircle(),
                     sglEllipse(), sglArc())
                   - SGL_LINE: draw only borders of graphics elements (lines)
                   - SGL_FILL: draw filled elements (default)

  ERRORS:
   - SGL_INVALID_ENUM
    mode is not an accepted value.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglAreaMode() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglAreaMode(sglEAreaMode mode);

/// Point size specification.
/**
  Sets the size of points for subsequent operations. Points are
  rendered as squares with [size x size] pixel dimensions.

  @param size [in] point size in pixels

  ERRORS:
   - SGL_INVALID_VALUE
    size is not positive.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPointSize() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglPointSize(float size);


/// Enabling SGL capabilities.
/**
  Enables SGL capabilities given as a bitmask.

 @kparam cap [in] capabilities bitmask; only SGL_DEPTH_TEST so far (off by default)
kk
  ERRORS:
   - SGL_INVALID_ENUM
    Generated if cap is not an accepted value.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEnable() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglEnable(sglEEnableFlags cap);

/// Disabling SGL capabilities.
/**
  Disables SGL capabilities given as a bitmask.

 @param cap [in] capabilities bitmask; only SGL_DEPTH_TEST so far (off by default)

  ERRORS:
   - SGL_INVALID_ENUM
    Generated if cap is not an accepted value.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglDisable() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglDisable(sglEEnableFlags cap);

/// Starting scene description.
/**
  Denotes the start of scene specification. The scene is initially empty.
  Any primitives specified by sglBegin(SGL_POLYGON) / sglEnd() or sglSphere()
  will be added to the scene primitive lists. No transformations are applied to
  the primitive vertices during the scene specification.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglBeginScene() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglBeginScene();

/// Ending scene description.
/**
  Denotes the end of scene specification.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEndScene() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglEndScene();

/// Sphere definition.
/**
  Adds a sphere primitive to the primitive list.

  @param x [in] sphere center x coordinate
  @param y [in] sphere center y coordinate
  @param z [in] sphere center z coordinate
  @param radius [in] sphere radius

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglSphere() is called within a
    sglBegin() / sglEnd() sequence or sglSphere() is called outside
    sglBeginScene() / sglEndScene() sequence.
 */
void sglSphere(const float x,
               const float y,
               const float z,
               const float radius);


/// Surface material specification.
/**
  Sets the material properties for subsequent graphics primitives specification.

  SGL employs the popular Phong model. Usually, 0 <= Kd <= 1 and 0 <= Ks <= 1,
  though it is not required that Kd + Ks == 1.
  Transmitting objects (T > 0) are considered to have two sides for algorithms
  that need these (normally objects are treated as having only one side).

  @param r [in] the red color component
  @param g [in] the green color component
  @param b [in] the blue color component
  @param kd [in] the diffuse coefficient
  @param ks [in] the specular coefficient
  @param shine [in] Phong cosine power for highlights
  @param T [in] the transmittance coefficient (fraction of contribution of the
                transmitting/refracted ray)
  @param ior [in] index of refraction

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMaterial() is called within a
    sglBegin() / sglEnd() sequence.
 */
void sglMaterial(const float r,
                 const float g,
                 const float b,
                 const float kd,
                 const float ks,
                 const float shine,
                 const float T,
                 const float ior);


/// Point light specification.
/**
  Adds a point light to the scene.

  @param x [in] x coordinate of the position of the light.
  @param y [in] y coordinate of the position of the light.
  @param z [in] z coordinate of the position of the light.
  @param r [in] light intensity in the red channel.
  @param g [in] light intensity in the green channel.
  @param b [in] light intensity in the blue channel.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPointLight() is called within a
    sglBegin() / sglEnd() sequence or sglPointLight() is called outside
    sglBeginScene() / sglEndScene() sequence.
 */
void sglPointLight(const float x,
                   const float y,
                   const float z,
                   const float r,
                   const float g,
                   const float b);


/// Rendering the image (ray tracing).
/**
  Computes an image of the scene using ray tracing.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRayTraceScene() is called within a
    sglBegin() / sglEnd() sequence or sglRayTraceScene() is called within a
    sglBeginScene() / sglEndScene() sequence.
*/
void sglRayTraceScene();

/// Rendering the image (ray tracing).
/**
  Computes an image of the scene using rasterization.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRasterizeScene() is called within a
    sglBegin() / sglEnd() sequence or sglRasterizeScene() is called within a
    sglBeginScene() / sglEndScene() sequence.
*/
void sglRasterizeScene();


/// Emissive material specification.
/**

  Sets the emissive properties for subsequent area lights geometry (SGL_POLYGONs)
  specification. The geometry represents patches of a single area light until
  another call to sglEmissiveMaterial() occurs. Only triangle patches need to be
  supported.

  The total energy emitted from one area light patch is (r, g, b) * patch_area.
  The attenuation with the distance d is 1 / (c0 + c1*d + c2*d^2).

  @param r [in] the red color component of light intensity per unit area
  @param g [in] the green color component of light intensity per unit area
  @param b [in] the blue color component of light intensity per unit area
  @param c0 [in] the constant attenuation coefficient
  @param c1 [in] the linear attenuation coefficient
  @param c2 [in] the quadratic attenuation coefficient

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEmissiveMaterial() is called within
    a sglBegin() / sglEnd() sequence.
 */
void sglEmissiveMaterial(const float r,
                         const float g,
                         const float b,
                         const float c0,
                         const float c1,
                         const float c2);

/// Environment map specification.
/**
  Sets the HDR environment map defining the "background" using a rectangular
  texture. If defined it replaces the background color (set with sglClearColor())
  for both primary and secondary rays.

  @param width [in] texture width
  @param height [in] texture height
  @param texels [in] texture elements (width*height RGB float triplets)

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEnvironmentMap() is called within a
    sglBegin() / sglEnd() sequence.
*/
void sglEnvironmentMap(const int width,
                       const int height,
                       float *texels);

#ifdef __cplusplus
}
#endif

#endif /* _SGL_H_ */
