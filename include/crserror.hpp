#pragma once

#if !PERSISTENCEDB_DLL

enum Err
{
   ERR_SUCCESS = 0

   ,  ERR_FAILURE
   ,  ERR_PARALLEL
   ,  ERR_STRAIGHT
   ,  ERR_NULL_VECTOR
   ,  ERR_BAD_ARGUMENTS
   ,  ERR_SINGULAR
   ,  ERR_NOT_APPLICABLE      // See also ERR_NOT_HANDLED_HERE
   ,  ERR_PERPENDICULAR
   ,  ERR_TOO_SHORT
   ,  ERR_SOME_ATTRIBUTES_UNDEFINED
   ,  ERR_NOT_FOUND
   ,  ERR_CONSTRAINTS_UNSATISFIED
   ,  ERR_DUPLICATE_DATA
   ,  ERR_INTERSECTS
   ,  ERR_OVERLAP
   ,  ERR_NOT_ONE_LOOP
   ,  ERR_NO_LOOPS
   ,  ERR_TOO_MANY_CURVES
   ,  ERR_NO_CURVES
   ,  ERR_OPEN_LOOP
   ,  ERR_CLOSED_LOOP
   ,  ERR_NOT_HORIZONTAL
   ,  ERR_NOT_VERTICAL
   ,  ERR_NO_SHARED_HISTORY
   ,  ERR_DATA_CORRUPT
   ,  ERR_OUT_OF_ORDER
   ,  ERR_OUTSIDE_OF_AREA
   ,  ERR_SEW_MISSED_TARGET
   ,  ERR_SEW_TARGET_TOO_LOW
   ,  ERR_SEW_TANGENT_TO_TARGET
   ,  ERR_HOLE_IN_ROOF_FAILED
   ,  ERR_NOT_MONOTONE
   ,  ERR_NOT_ONE_CCW_LOOP
   ,  ERR_VIEW_NAME_EMPTY
   ,  ERR_VIEW_NAME_DUPLICATE
   ,  ERR_SHEET_NUMBER_EMPTY
   ,  ERR_SHEET_NUMBER_DUPLICATE
   ,  ERR_MATRIX_TOO_LARGE
   ,  ERR_INSTANCE_PARAMETER
   ,  ERR_DEGENERATE_FACE
   ,  ERR_TRIANGULATION_FAILED
   ,  ERR_USER_ABORT
   ,  ERR_NO_ELEMENTS_TO_MOVE
   ,  ERR_CONTENTION // central model is busy, different from ERR_WORKSET_CONTENTION
   ,  ERR_CONTAINED_LOOPS
   ,  ERR_DELAYED
   ,  ERR_TOO_MUCH_PROPAGATION
   ,  ERR_CAN_NOT_MERGE_FACES
   ,  ERR_POST_FAILURE_INFO
   ,  ERR_TIMEOUT
   ,  ERR_NOT_ENOUGH_POINTS
   ,  ERR_BUSY
   ,  ERR_NOT_LATEST
   ,  ERR_EDITABILITY_UNKNOWN
   ,  ERR_AT_RISK
   ,  ERR_REFS_NOT_REGENERATED
   ,  ERR_CANT_OPEN_FILE
   ,  ERR_CENTRAL_FILE_ACCESS // central model is missing/unreachable
   ,  ERR_TYPE_PARAMETER
   ,  ERR_UNLOADED
   ,  ERR_NOT_COMPATIBLE
	,  ERR_NOT_EXACTLY_ORTHOGONAL
   ,  ERR_PERMISSION_FAILURE
   ,  ERR_REQUEST_DISAPPEARED
   ,  ERR_ELEMENTS_DISAPPEARED
   ,  ERR_NESTED_LOOPS
   ,  ERR_INTERRUPTED_STEP
   ,  ERR_INTERSECTS_INTERNAL
   ,  ERR_INTERSECTS_EXTERNAL
   ,  ERR_ENDS_JOINED
   ,  ERR_PARAM_AMBIGUOUS_LOCATION
   ,  ERR_PARAM_OPTION_CONFLICT
   ,  ERR_PARAM_ERROR
   ,  ERR_HOSTPANEL_OBJ_ALREADY_ASSIGNED
   ,  ERR_PARAM_NOT_PLACED
   ,  ERR_PARAM_NOT_ENCLOSED
   ,  ERR_PARAM_REDUNDANT_ROOM
   ,  ERR_PARAM_REDUNDANT_AREA
   ,  ERR_NOT_STRUCTURAL_FAMILY_INSTANCE
   ,  ERR_CANT_FIND_DEFAULT_VALUE
   ,  ERR_MANY_CCW_LOOPS
   ,  ERR_NO_DIRECTION_CURVE
   ,  ERR_OUT_OF_BOUNDS_CANNOT_PROCEED
   ,  ERR_CANCELLED
   ,  ERR_DISPLAYABLE
   ,  ERR_PAUSE
   ,  ERR_NOT_IMPLEMENTED
   ,  ERR_CONTINUE
   ,  ERR_DELETED_IN_CENTRAL
   ,  ERR_LINKED_ROOM_PROBLEM
   ,  ERR_USER_CANCEL_COZ_AT_RISK
   ,  ERR_PARAM_NOT_COMPUTED
   ,  ERR_NOT_ONE_CURVE
   ,  ERR_PARAM_VARIES
   ,  ERR_DISABLED
   ,  ERR_MISSING_FROM_HOST
   // a function discovered that it cannot proceed BEFORE it changed anything in it non-const arguments
   ,  ERR_CONST_PRECHECK_FAILED
   ,  ERR_NO_GEOMETRY
   ,  ERR_CENTRAL_FILE_READ_ONLY
   // geometry was there but disppeared due to some legitimate reasons
   ,  ERR_NO_GEOMETRY_DORMANT
   ,  ERR_CIRCULAR_LINK
   ,  ERR_NOT_HANDLED_HERE   // Generic. Intended as "I don't know what to do - caller handle it!"
   ,  ERR_MANY_DIRECTION_CURVES
   ,  ERR_BAD_PARAMETER_RANGE
   ,  ERR_PARAM_SHOW_AS_AUTOMATIC
   ,  ERR_PARAM_REDUNDANT_SPACE
   ,  ERR_ALREADY_DONE
   ,  ERR_COPY_FAILURE
   ,  ERR_SINGULAR_BAD  // singularity which cannot (could not) be succesfully processed
   ,  ERR_BATCH_PARTIALLY_SUCCEEDED // Batch Printing/Exporting only partially succeeded
   ,  ERR_CANNOT_USE_WORKPLANE
   ,  ERR_DELETED_IN_LINK
   ,  ERR_TOO_FAR
   ,  ERR_DEVICE_LOST
   ,  ERR_SERVER_COMMAND_SKIPPED       // Current command to Revit server is skipped
   ,  ERR_REVIT_SERVER_NOT_ACCESSIBLE  // Revit server is not accessible due to communication issue
   ,  ERR_REVIT_SERVER_CORRUPT         // Revit server is corrupted due to error with super lock file
   ,  ERR_CENTRAL_MODEL_CONTENTION // Central model on server is not accessible due to contention with other clients. It seems duplicated with ERR_CONTENTION,
                                   // but it is unnecessary to merge them, because file-based workflow, which uses ERR_CONTENTION, could eventually migrate to
                                   // new RS Comm code flow.
   ,  ERR_CENTRAL_MODEL_ACCESSDENIED   // Central model on server is denied by administrator
   ,  ERR_CENTRAL_MODEL_CORRUPT        // Central model on server is corrupted due to error with Data/Permissions
   ,  ERR_CENTRAL_MODEL_INCOMPATIBLE_OVERWRITTEN // Central model on server is overwritten by other user
   ,  ERR_CENTRAL_MODEL_OLD_VERSION        // Central model on server could not be overwritten due to old version
   ,  ERR_CENTRAL_MODEL_PERMISSIONS_OBSOLETED // Permissions of a central model is obsoleted
   ,  ERR_CENTRAL_MODEL_PERMISSIONS_UPGRADED  // Permissions of a central model is upgraded
   ,  ERR_SIZINGFAILURE_CALCULATION_NONE
   ,  ERR_SIZINGFAILURE_CALCULATION_FLOWONLY
   ,  ERR_CENTRAL_MODEL_PERMISSIONS_RECONSTRUCTED  // Permissions of a central model is reconstructed
   ,  ERR_CENTRAL_MODEL_PERMISSIONS_INFUTUREFORMAT // Permissions of a central model is in future format
   ,  ERR_CENTRAL_MODEL_PERMISSIONS_INOLDERFORMAT  // Permissions of a central model is in older format
         // edge condition profile was generated but without a chamfer/miter - its requested size was too big
   ,  ERR_CHAMFER_IGNORED
         // edge condition profile was not generated as too big gap between adjacent panels was requested
   ,  ERR_TOO_BIG_GAP
         // edge condition profile was not generated as its parameters are not consistent with the 'thickness' of the host
   ,  ERR_MULTI_VALUES                 // Show "Multi Value" for parameters which have multiple values
   ,  ERR_INCOMPATIBLE_THICKNESS
   ,  ERR_CENTRAL_MODEL_HAS_NEW_VERSION   // The central model has been updated by other clients
   ,  ERR_CENTRAL_MODEL_MISSING        // Central model is missing on server
   ,  ERR_SERVER_RESOURCE_UNAUTHORIZED   // Current user is unauthorized to access resource
   ,  ERR_UNAUTHENTICATED_USER // Revit requires SSO information to proceed, but the user is not logged in to SSO.
   ,  ERR_SIZELOOKUPTABLE_NOTFOUND       // Size lookup table is not found
   ,  ERR_CANNOT_MOVE_ELEMENTS        // Cannot move elements
   ,  ERR_WORKSET_CONTENTION          // Workset's permission has been changed before the current operation submission, different from ERR_CONTENTION
   ,  ERR_SERVER_LOW_DISK_SPACE       // The free disk space is running low at Revit Server    
   ,  ERR_DELETE_FAILURE
   ,  ERR_UNUSED_POINTS
   ,  ERR_CENTRAL_MODEL_ALREADY_EXISTS // The Model already exists remotely, and cannot be overwritten.  Conceptually different from OldVersion.
   ,  ERR_COLLABORATION_EXTERN_RES_UNAVAILABLE // Collaboration Not Available due to External Resource failure (e.g., Amazon S3).
   ,  ERR_PARAM_CALCULATING            // The parameter is asynchronously calculating.
   ,  ERR_PARAM_WORKER_FAILED          // The background worker that asynchronously calculates the parameter value has failed. 
   ,  ERR_CENTRAL_FILE_CORRUPT         // central file is corrupt, different from ERR_CENTRAL_MODEL_CORRUPT
   ,  ERR_COORDINATES_ALREADY_SHARED   // Shared Coordinate System Already Synchronized
   ,  ERR_ACQUIRE_COORDINATES_FROM_MULTIPLE_LINK_INSTANCES  // Cannot acquire coordinates from a model placed multiple times
   ,  ERR_RECURSIVE
   ,  ERR_EXHAUSTED
   ,  ERR_ALERTED
   // Add new error codes above this line
};

#endif