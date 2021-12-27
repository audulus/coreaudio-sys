
typedef uint32_t AUAudioFrameCount;
typedef float AUValue;
typedef uint64_t AUParameterAddress;

// forward declaration
union AURenderEvent;

// =================================================================================================
// Realtime events - parameters and MIDI

/*
/// Describes the type of a render event.
typedef NS_ENUM(uint8_t, AURenderEventType) {
	AURenderEventParameter		= 1,
	AURenderEventParameterRamp	= 2,
	AURenderEventMIDI			= 8,
	AURenderEventMIDISysEx		= 9,
	AURenderEventMIDIEventList  = 10
};
*/

typedef uint8_t AURenderEventType;
#define AURenderEventParameter 1
#define AURenderEventParameterRamp 2
#define AURenderEventMIDI 8
#define AURenderEventMIDISysEx 9
#define AURenderEventMIDIEventList 10

#pragma pack(4)
///	Common header for an AURenderEvent.
typedef struct AURenderEventHeader {
	union AURenderEvent *__nullable next;		//!< The next event in a linked list of events.
	AUEventSampleTime		eventSampleTime;	//!< The sample time at which the event is scheduled to occur.
	AURenderEventType		eventType;			//!< The type of the event.
	uint8_t					reserved;			//!< Must be 0.
} AURenderEventHeader;

/// Describes a scheduled parameter change.
typedef struct AUParameterEvent {
	union AURenderEvent *__nullable next;		//!< The next event in a linked list of events.
	AUEventSampleTime		eventSampleTime;	//!< The sample time at which the event is scheduled to occur.
	AURenderEventType		eventType;			//!< AURenderEventParameter or AURenderEventParameterRamp.
	uint8_t					reserved[3];		//!< Must be 0.
	AUAudioFrameCount		rampDurationSampleFrames;
												//!< If greater than 0, the event is a parameter ramp; 
												/// 	 should be 0 for a non-ramped event.
	AUParameterAddress		parameterAddress;	//!< The parameter to change.
	AUValue					value;				//!< If ramped, the parameter value at the
												///	end of the ramp; for a non-ramped event,
												///	the new value.
} AUParameterEvent;

/// Describes a single scheduled MIDI event.
typedef struct AUMIDIEvent {
	union AURenderEvent *__nullable next;		//!< The next event in a linked list of events.
	AUEventSampleTime		eventSampleTime;	//!< The sample time at which the event is scheduled to occur.
	AURenderEventType		eventType;			//!< AURenderEventMIDI or AURenderEventMIDISysEx.
	uint8_t					reserved;			//!< Must be 0.
	uint16_t				length;				//!< The number of valid MIDI bytes in the data field.
												/// 1, 2 or 3 for most MIDI events, but can be longer
												/// for system-exclusive (sys-ex) events.
	uint8_t					cable;				//!< The virtual cable number.
	uint8_t					data[3];			//!< The bytes of the MIDI event. Running status will not be used.
} AUMIDIEvent;

/// Describes a single scheduled MIDIEventList.
typedef struct AUMIDIEventList {
	union AURenderEvent *__nullable next;		//!< The next event in a linked list of events.
	AUEventSampleTime		eventSampleTime;	//!< The sample time at which the event is scheduled to occur.
	AURenderEventType		eventType;			//!< AURenderEventMIDI or AURenderEventMIDISysEx.
	uint8_t					reserved;			//!< Must be 0.
	uint8_t					cable;				//!< The virtual cable number.
	MIDIEventList			eventList;			//!< A structure containing UMP packets.
} AUMIDIEventList;

typedef union AURenderEvent {
	AURenderEventHeader		head;
	AUParameterEvent		parameter;
	AUMIDIEvent				MIDI;
	AUMIDIEventList			MIDIEventsList;
} AURenderEvent;
#pragma pack()

