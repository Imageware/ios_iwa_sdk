// Copyright (c) 2020 RedRock Biometrics. All rights reserved.
#ifndef __PALM_ID_H__
#define __PALM_ID_H__

#ifndef PALM_EXPORT
#ifdef _MSC_VER
#define PALM_EXPORT __declspec(dllexport)
#else
#define PALM_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef PALM_CALL
#ifdef _MSC_VER
#define PALM_CALL __stdcall
#else
#define PALM_CALL
#endif
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PalmIDHandle {
  void* impl;
} PalmIDHandle;

/**
 * Possible return values from the various API functions.
 */
typedef enum _ePalmStatus {
  /**
   * The desired operation completed successfully.
   */
  ePalm_Success = 0,

  /**
   * The function timed-out before completing the desired operation.
   */
  ePalm_Timeout = 1,

  /**
  * Some allocated memory was not freed.
  */
  ePalm_MemoryLeak = 2,

  /**
   * There was an error completing the desired operation, but for an unknown
   * reasons.
   */
  ePalm_UnknownError = 0x8000,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_InvalidHandle = 0x8001,

  /**
   * One of the provided arguments was invalid or incomplete.
   */
  ePalm_InvalidArgument = 0x8002,

  /**
   * Insufficient memory in buffer.
   */
  ePalm_OutOfMemory = 0x8003,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_UnexpectedRequest = 0x8004,

  /**
   * The license id used was invalid.
   */
  ePalm_InvalidLicense = 0x8005,

  /**
   * The model does not exist or is invalid.
   */
  ePalm_InvalidModel = 0x8006,

  /**
   * Error connecting to API server.
   */
  ePalm_ServerConnectionError = 0x8007,

  /**
   * Error in data serialization.
   */
  ePalm_SerializationError = 0x8008
} ePalmStatus;

/**
* The type of palm to use.
*/
typedef enum _PalmType {
  /**
  * A visible capture of the palm print.
  */
  PalmType_Print = 0,

  /**
  * An infrared capture of the palm veins.
  */
  PalmType_Vein = 1
} PalmType;

typedef struct _PalmImage {
  /**
   * Type of the image.
   */
  PalmType type;

  /**
   * Pointer to the raw image data.
   */
  uint8_t* data;

  /**
   * Bit depth of the image in bits.
   */
  uint32_t depth;

  /**
   * Width of the image in pixels.
   */
  uint32_t width;

  /**
   * Height of the image in pixels.
   */
  uint32_t height;

  /**
   * Size of a row of pixels in bytes, including any padding at end.
   */
  uint32_t stride;

  /**
   * Number of bytes offset from the beginning of data to the start of image.
   */
  uint32_t offset;

  /**
   * The time at which the frame was captured in microseconds.
   */
  int64_t timestamp;

  /**
   * If non-zero, the capturing device has active illumination such as infrared LEDs or torch.
   */
  int8_t active_lighting;

  /**
   * If non-zero, the camera is facing away from the user.
   */
  int8_t rear_facing_camera;
} PalmImage;

/**
 * A camera device frame.
 */
typedef struct _PalmFrame {
  /**
  * Number of images associated with this frame.
  */
  uint32_t num_images;

  /**
   * Pointers to images associated with this frame. Support up to 8 images but only the first 'num_images' are allocated and used.
   */
  PalmImage* images[8];
} PalmFrame;

typedef enum _PalmSidedness {
  /**
  * The handedness could not be determined
  */
  PalmSidedness_Undetermined = -1,

  /**
  * The palm is a left hand
  */
  PalmSidedness_Left = 0,

  /**
  * The palm is a right hand
  */
  PalmSidedness_Right = 1
} PalmSidedness;

/**
* Strictness refers to how accurately the palm needs to be aligned to the guide, as well as other criteria,
* in order to capture it.  A higher strictness means more accurate matching, but may be more difficult for
* a user to position their hand for capture.  A lower strictness means a palm can be placed in more places
* within the camera's field of view, but matching accuracy may be lower.
*/
typedef enum _PalmStrictness {
  /**
  * The palm cannot be captured.
  */
  PalmStrictness_None = -1,

  /**
  * The minimum possible strictness to capture a palm. (1/5)
  */
  PalmStrictness_Minimum = 0,

  /**
  * A high strictness criterion recommended for palm authentication captures. (2/5)
  */
  PalmStrictness_Authenticate = 1,

  /**
  * A medium strictness to capture a palm. (3/5)
  */
  PalmStrictness_Medium = 2,

  /**
  * A high strictness requirement recommended for palm enrollment captures. (4/5)
  */
  PalmStrictness_Enrollment = 3,

  /**
  * The maximum possible strictness to capture a palm. (5/5)
  */
  PalmStrictness_Maximum = 4
} PalmStrictness;

typedef enum _PalmLiveness {
  /**
  * No liveness is preformed.
  */
  PalmLiveness_None = 0,

  /**
  * After palm capture, user is prompted to make a fist.
  */
  PalmLiveness_Fist = 1,

  /**
  * After palm capture, a high-resolution image with active lighting is captured.
  */
  PalmLiveness_HiRes = 2,

  /**
  * Combination of PalmLiveness_HiRes followed by PalmLiveness_Fist.
  */
  PalmLiveness_HiRes_Fist = 3,
} PalmLiveness;

typedef struct _PalmModelID {
  /**
   * Unique identifier for palm model
   */
  uint8_t id[20];
} PalmModelID;

/**
 * A 2-D point.
 */
typedef struct _PalmPoint2D {
  float x;
  float y;
} PalmPoint2D;

/**
 * A 2-D quadrilateral described by four 2-D points.
 */
typedef struct _PalmQuad2D {
  PalmPoint2D a;
  PalmPoint2D b;
  PalmPoint2D c;
  PalmPoint2D d;
} PalmQuad2D;

typedef struct _PalmDetected {
  /**
   * A quadrilateral in image coordinates enclosing the palm.
   */
  PalmQuad2D quad;

  /**
   * Timestamp of when image with detected palm was captured in microseconds.
   */
  int64_t timestamp;

  /**
   * The highest strictness level that the palm could be immediately captured.
   * If no palm can be captured, this value will be PalmStrictness_None.
   */
  PalmStrictness readiness;

  /**
   * The quality of the palm.
   */
  float quality;

  /**
   * The brightness of the palm.
   */
  float brightness;

  /**
   * The centerness of the palm.
   */
  float centerness;

  /**
   * A scaler between 0 and 1 to indicate the likelihood of the detected palm being left or right (0 is definitely left and 1 is definitely right).
   */
  float sidedness;
} PalmDetected;

/**
 * The palms detected in the latest frame.
 */
typedef struct _PalmsDetected {
  /**
   * The number of palms detected.
   */
  uint32_t num_palms;

  /**
   * An array of detected palms, consisting of num_detected palms.
   */
  PalmDetected* palms;
} PalmsDetected;

/**
 * An individual matching result.
 */
typedef struct _PalmMatch {
  /**
   * The id of the previously registered model that is matched.
   */
  PalmModelID matched_id;

  /**
   * The matching score.
   */
  float score;

  /**
   * When the current live scan and previously registered model matches,
   * the registered model may be updated if the update_model flag was set
   * to true when PalmID_Match is called. When the model is updated, this
   * field will be non-zero. In this case, use PalmID_GetModel to retrieve
   * and save the updated model for future usage.
   */
  uint8_t updated;
} PalmMatch;

/**
 * The matching result.
 */
typedef struct _PalmMatchingResult {
  /**
   * The id of the current live model scan.
   */
  PalmModelID live_id;

  /**
   * The type of matching used.
   */
  PalmType type;

  /**
   * The number of palms that passed matching.
   */
  int32_t num_matches;

  /**
   * An array of num_matches matching results. Only matches with score above
   * the requested threshold are returned.
   */
  PalmMatch* matches;
} PalmMatchingResult;

/**
 * The capture result.
 */
typedef struct _PalmCaptureResult {
  /**
   * A unique identifier for the captured palm
   */
  PalmModelID model_id;

  /**
   * A flag to indicate if palm model is for left or right.
   */
  PalmSidedness sidedness;

  /**
   * A bitcode to indicate palm type. 0-Unknown, 1-Print, 2-Vein, 3-Print and Vein
   */
  uint8_t type;

  /**
   * A pointer to the data stream for this model. This will be null if capture failed.
   */
  void* data;

  /**
  * The size of the model data in bytes. This will be 0 if capture failed.
  */
  uint32_t data_size;
} PalmCaptureResult;

typedef enum _PalmLivenessStatus {
  /**
  * It could not be determined if the detected palm is a live one or not.
  */
  PalmLivenessStatus_Undetermined = -1,

  /**
  * The palm passes the liveness check.
  */
  PalmLivenessStatus_Pass = 0,

  /**
  * The palm fails the liveness check.
  */
  PalmLivenessStatus_Fail = 1,

  /**
  * The palm fails because the background is too bright.
  */
  PalmLivenessStatus_Fail_Background = 2
} PalmLivenessStatus;

/**
* The liveness check result.
*/
typedef struct _PalmLivenessResult {
  /**
  * The liveness check status;
  */
  PalmLivenessStatus status;

  /**
  * The model id of the palm
  */
  PalmModelID model_id;
} PalmLivenessResult;

/**
* The GetModel result.
*/
typedef struct _PalmGetModelResult {
  /**
   * The id of the requested model
   */
  PalmModelID model_id;

  /**
   * A pointer to the data stream for this model. This will be null if no model was found.
   */
  void* data;

  /**
  * The size of the model data in bytes. This will be 0 if no model was found.
  */
  uint32_t data_size;
} PalmGetModelResult;

/**
 * The possible message types.
 */
typedef enum _ePalmMessageType {
  /**
   * No message was available.
   */
  MessageNone = 0,

  /**
   * The message corresponds to palms being detected.
   */
  MessagePalmsDetected = 1,

  /**
   * The message corresponds to palm matching result.
   */
  MessageMatchingResult = 2,

  /**
   * The message corresponds to palm capture result.
   */
  MessageCaptureResult = 3,

  /**
   * The message corresponds to palm matching started.
   */
  MessageMatchingStarted = 4,

  /**
   * The message corresponds to palm matching finished.
   */
  MessageMatchingFinished = 5,

  /**
   * The message corresponds to palm capture started.
   */
  MessageCaptureStarted = 6,

  /**
   * The message corresponds to palm capture finished.
   */
  MessageCaptureFinished = 7,

  /**
   * The message corresponds to add palm model.
   */
  MessageAddModelResult = 9,

  /**
   * The message corresponds to remove palm model.
   */
  MessageRemoveModelResult = 10,

  /**
  * The message corresponds to palm liveness check result.
  */
  MessageLivenessResult = 11,

  /**
  * The message corresponds to palm liveness check started.
  */
  MessageLivenessStarted = 12,

  /**
  * The message corresponds to palm liveness check finished.
  */
  MessageLivenessFinished = 13,

  /**
  * The message corresponds to get model result.
  */
  MessageGetModelResult = 14
} ePalmMessageType;

/**
 * The result of a request to add a palm model
 */
typedef struct _PalmAddModelResult {
  /**
   * The ID of the model that was attempted to be added.
   */
  PalmModelID model_id;

  /**
   * Non-zero if the model was successfully added to the database.
   */
  uint8_t success;
} PalmAddModelResult;

/**
* The result of a request to remove a palm model
*/
typedef struct _PalmRemoveModelResult {
  /**
  * The ID of the model that was attempted to be removed.
  */
  PalmModelID model_id;

  /**
  * Non-zero if the model was successfully removed from the database.
  */
  uint8_t success;
} PalmRemoveModelResult;

typedef struct _PalmMessage {
  /**
   * The message type of this message.
   */
  ePalmMessageType type;

  /**
   * The error status. If the value is not ePalm_Success, then an error occurs when the message is generated.
   */
  ePalmStatus status;

  /**
   * A pointer to the message corresponding to the message type.
   */
  union {
    void* pointer;
    PalmsDetected* palms_detected;
    PalmCaptureResult* capture_result;
    PalmMatchingResult* matching_result;
    PalmLivenessResult* liveness_result;
    PalmAddModelResult* add_model_result;
    PalmRemoveModelResult* remove_model_result;
    PalmGetModelResult* get_model_result;
  } message;
} PalmMessage;

/**
 * Set configuration parameters. A parameter is specified by two strings [name, value].
 * Call this function before calling PalmID_Create.
 *
 * @param key A pointer to parameter name string. The string is expected to end with '\0'.
 * @param value A pointer to parameter value string. The string is expected to end with '\0'.
 *
 * For possible key values, see 'PalmID_GetConfig'.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_SetConfig(const char* key, const char* value);

/**
 * Get configuration parameters.
 *
 * @param key A pointer to parameter name string. The string is expected to end with '\0'.
 * @param value A pointer to hold the output parameter value string. The caller needs to allocate the memory.
 * @param value_size A pointer to an integer to hold the output parameter value string size. This parameter
 * is updated upon return with the actual number of bytes written to the value pointer.
 * @param null_terminate A boolean value on whether or not to include a null-termination character ('\0') at
 * the end of the value string (0 is no, otherwise yes).
 *
 * @note: If the size of the value buffer is insufficient to hold the entire value, this function will
 * return an ePalm_InsufficientMemory error, and the value_size parameter will be updated with the actual
 * size needed to store the results.
 *
 * Possible keys include:
 *   "version":         The full version number of the SDK and build (read-only).
 *   "major_version":   The major version number of the SDK (read-only).
 *   "minor_version":   The minor version number of the SDK (read-only).
 *   "license_id":      The license id required for authentication.
 *   "encryption_key":  A string used to encrypt/decrypt models.
 *   "rotation_angle":  Rotation of the palm in degrees.  Fingers pointing up in the image is 0 degrees
 *                      and increases clockwise to 360 degrees.  Negative angles are okay as well.
 *                      The value "auto" may also be used so the SDK can automatically choose the angle.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_GetConfig(const char* key, char* value, int32_t* value_size, int null_terminate);

/**
 * Create a new palm biometrics handle that is passed to most of the Palm biometrics
 * functions.
 *
 * @param handle A pointer to a palm biometrics handle.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_Create(PalmIDHandle* handle);

/**
 * Destroy a palm biometrics handle.
 *
 * @param handle A palm biometrics handle.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_Destroy(PalmIDHandle* handle);

/**
 * Current time in microseconds.
 *
 * @return The current time in microseconds. A negative value indicates failure.
 */
PALM_EXPORT int64_t PALM_CALL PalmID_Now(void);

/**
 * Provide a camera frame for processing.
 *
 * @param handle A palm biometrics handle.
 * @param frame The frame acquired through the Camera SDK or from an
 *              alternate image-capture interface.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_ProcessFrame(PalmIDHandle handle, const PalmFrame* frame);

/**
 * Attempt to create a new palm model that will extracted from the frame sequence provided
 * by PalmID_ProcessFrame(). The new model will be returned as a PalmCaptureResult.
 *
 * @param handle A palm biometrics handle.
 * @param strictness The strictness criterion for a palm capture.
 * @param liveness Liveness criterion for a successful palm capture.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_Capture(PalmIDHandle handle, PalmStrictness strictness, PalmLiveness liveness);

/**
* Obtains a serialized model that can be saved to disk for future matches.
* After calling this function, the model will be returned in a GetModelResult message.
*
* @param handle A palm biometrics handle.
* @param model_id A palm model ID for the desired palm model.
*/
PALM_EXPORT ePalmStatus PALM_CALL PalmID_GetModel(PalmIDHandle handle, const PalmModelID* model_id);

/**
 * Attempt to add a newly created palm model if it doesn't already exist so that it is
 * available for matching.
 *
 * @param handle A palm biometrics handle.
 * @param model_data A pointer to palm model data stream.
 * @param model_data_size The size of palm model data stream in bytes.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_AddModel(PalmIDHandle handle, const void* model_data, uint32_t model_data_size);

/**
* Attempt to remove an existing palm model so that it is no longer available for matching.
*
* @param handle A palm biometrics handle.
* @param model_id Pointer to the id of the model to be removed.
*/
PALM_EXPORT ePalmStatus PALM_CALL PalmID_RemoveModel(PalmIDHandle handle, const PalmModelID* model_id);

/**
 * Attempt to match a live palm against a set of existing palm model(s). The result(s)
 * will be returned in a PalmMatchingResult message.
 *
 * Successful matches may update existing models to improve future matches.
 * To keep models up to date, use 'PalmID_GetModel' to update stored models.
 *
 * @param handle A palm biometrics handle.
 * @param live_model    A pointer to a live model to match against the database of models
 * @param model_ids     A pointer to the IDs of models that will be matched to when a palm is detected.
 * @param num_model_ids The number of models stored in models array.
 * @param type  The type of palm being matched. When a model consists of multiple images of different types,
 *              only the images correspond to the requested type are used in matching. To match multiple
 *              types (modalities), call PalmID_Match multiple times.
 * @param score_threshold  A threshold in [0, 1] for returned matching results. Only matches with scores higher
 *              than the threshold will be returned.
 * @param update_model  A flag to indicate whether to update the to be matched model when the match passes. Set
 *              this parameter to zero or non-zero to disable or enable the model update. Note that the model is
 *              not always updated when the match passes, other conditions need to be satisfied as well. When the
 *              model is indeed updated, the 'updated' flag in PalmMatch will be set to non-zero. In this case,
 *              the user is recommended to call PalmID_GetModel to retrieve the updated model.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_Match(PalmIDHandle handle, const void* live_model, int32_t live_model_size, const PalmModelID* model_ids, int32_t num_model_ids, PalmType type, float score_threshold, uint8_t update_model);

/**
 * Add a message to the message queue.  This is useful when palm capture and matching happen on
 * different systems and messages need to be passed between them, such as a client and server.
 *
 * If the message cannot be added to the queue due to format errors, an error status will be returned.
 *
 * @param handle A palm biometrics handle.
 * @param message A PalmMessage to be copied and sent to the message queue.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_QueueMessage(PalmIDHandle handle, PalmMessage message);

/**
 * Wait for a message to become available. The function will wait until either
 * a message becomes available or the specified timeout duration elapses.
 *
 * PalmMessages should be freed after use with 'PalmID_FreeMessage'
 *
 * @param handle A palm biometrics handle.
 * @param message A memory address of a PalmMessage to write into.
 * @param timeout The maximum length of time, in milliseconds, to wait for a
 *                message to become available.
 */
PALM_EXPORT ePalmStatus PALM_CALL PalmID_WaitMessage(PalmIDHandle handle, PalmMessage* message, int32_t timeout);

/**
* Deallocates a message returned from 'PalmID_WaitMessage'
*
* @param handle A palm biometrics handle.
* @param message The PalmMessage to free.
*/
PALM_EXPORT ePalmStatus PALM_CALL PalmID_FreeMessage(PalmIDHandle handle, PalmMessage message);

/**
* Insert a palm image to a palm frame. This function creates a new palm image of specified type and size and allocates memory
* for its image data buffer. If the frame already contains the maximum number of images, this function does nothing.
*
* @param frame   A palm frame to add the new image.
*        width   Image width
*        height  Image height
*        depth   Image depth in bits.  Must be 8 (Grayscale) or 24 (RGB)
*        channel Image channel source
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddImage(PalmFrame* frame, uint32_t width, uint32_t height, uint32_t depth, PalmType type);

/**
* Insert a palm image to a palm frame. This function creates a new palm image from a binary file such as a BMP, PNG, or JPG
* and allocates memory for its image data buffer. If the frame already contains the maximum number of images, this function does nothing.
*
* @param frame     A palm frame to add the new image.
*        file_data The binary file data
*        file_size Size of the file data
*        channel   Image channel source
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddImageFromFile(PalmFrame* frame, const void* file_data, uint32_t file_size, PalmType type);

/**
* Create an empty palm frame with no images.
*
* @param frame      A pointer to a palm frame that will be initialized as an empty frame with no images.

* @note call PalmFrame_AddImage to insert an image into a frame. Note that the memory for the palm frame and images are allocated
*       inside the PalmAPI shared library.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Create(PalmFrame* frame);

/**
* Free a palm frame and its images.
*
* @param frame A palm frame allocated inside Palm API shared library.
*
* @note This function releases the memories allocated for a frame and all its images.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Free(PalmFrame* frame);

#ifdef __cplusplus
}
#endif

#endif /* __PALM_ID_H__ */
