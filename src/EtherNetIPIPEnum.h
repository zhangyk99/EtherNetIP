//
// Created by 79933 on 2024/11/8.
//

#ifndef ETHERNETIP_EIPENUM_H
#define ETHERNETIP_EIPENUM_H

#include <vector>

namespace EtherNetIP {
    enum class CIPGeneralStatusCode {
        Success,
        ConnectionFailure,
        ResourceUnavailable,
        InvalidParameterValue,
        PathSegmentError,
        PathDestinationUnknown,
        PartialTransfer,
        ConnectionLost,
        ServiceNotSupported,
        InvalidAttributeValue,
        AttributeListError,
        AlreadyInRequestedModeState,
        ObjectStateConflict,
        ObjectAlreadyExists,
        AttributeNoSettable,
        PrivilegeViolation,
        DeviceStateConflict,
        ReplyDataTooLarge,
        FragmentationOfaPrimitiveValue,
        NotEnoughData,
        AttributeNotSupported,
        TooMuchData,
        ObjectDoesNotExist,
        ServiceFragmentationSequenceNotInProgress,
        NotStoreAttributeData,
        StoreOperationFailure,
        RoutingFailureRequestPacketTooLarge,
        RoutingFailureResponsePacketTooLarge,
        MissingAttributeListEntryData,
        InvalidAttributeValueList,
        EmbeddedServiceError,
        VendorSpecificError,
        InvalidParameter,
        WriteOnceValueOrMediumAlreadyWriten,
        InvalidReplyReceived,
        BufferOverflow,
        InvalidMessageFormat,
        KeyFailureInPath,
        PathSizeInvalid,
        UnexpectedAttributeInList,
        InvalidMemberID,
        MemberNotSettable,
        Group2OnlyServerGeneralFailure,
        UnknownModbusError,
        AttributeNotGettable
    };

    enum class CIPObjectLibrary {
        Identity = 1,
        MessageRouter = 2,
        DeviceNet = 3,
        Assembly = 4,
        Connection = 5,
        ConnectionManager = 6,
        Register = 7,
        DiscreteInputPoint = 8,
        DiscreteOutputPoint = 9,
        AnalogInputPoint = 10,
        AnalogOutputPoint = 11,
        PresenceSensing = 14,
        Parameter = 15,
        ParameterGroup = 16,
        Group = 18,
        DiscreteInputGroup = 29,
        DiscreteOutputGroup = 30,
        DiscreteGroup = 31,
        AnalogInputGroup = 32,
        AnalogOutputGroup = 33,
        AnalogGroup = 34,
        PositionSensor = 35,
        OptionControllerSupervisor = 36,
        PositionController = 37,
        BlockSequencer = 38,
        CommandBlock = 39,
        MotorData = 40,
        ControlSupervisor = 41,
        AcDcDrive = 42,
        AcknowledgeHandler = 43,
        Overload = 44,
        SoftStart = 45,
        Selection = 46,
        SDeviceSupervisor = 48,
        SAnalogSensor = 49,
        SAnalogActuator = 50,
        SSingleStageController = 51,
        SGasCalibration = 52,
        TripPoint = 53,
        File = 55,
        SPartialPressure = 56,
        SafetySupervisor = 57,
        SafetyValidator = 58,
        SafetyDiscreteOutputPoint = 59,
        SafetyDiscreteOutputGroup = 60,
        SafetyDiscreteInputPoint = 61,
        SafetyDiscreteInputGroup = 62,
        SafetyDualChannelOutput = 63,
        SSensorCalibration = 64,
        EventLog = 65,
        MotionAxis = 66,
        TimeSync = 67,
        Modbus = 68,
        DLR = 71,
        Qos = 72,
        ControlNet = 240,
        ControlNetKeeper = 241,
        ControlScheduling = 242,
        ConnectionConfiguration = 243,
        Port = 244,
        TCIIPInterface = 245,
        EtherNetLink = 246,
        CompoNetLink = 247,
        CompoNetRepeater = 248
    };

    enum class CIPServiceCodes {
        GetAttributesAll = 1,
        SetAttributeAll = 2,
        GetAttributeList = 3,
        SetAttributeList = 4,
        Reset = 5,
        Start = 6,
        Stop = 7,
        Create = 8,
        Delete = 9,
        MultipleServicePacket = 10,
        ApplyAttributes = 13,
        GetAttributeSingle = 14,
        SetAttributeSingle = 16,
        FindNextObjectInstance = 17,
        Restore = 21,
        Save = 22,
        Nop = 23,
        GetMember = 24,
        SetMember = 25,
        InsertMember = 26,
        RemoveMember = 27,
        GroupSync = 28,
        ForwardClose = 78,
        UnconnectedSend = 82,
        ForwardOpen = 84,
        LargeForwardOpen = 91
    };

    enum class CIPType {
        BOOL,
        SINT,
        INT,
        DINT,
        LINT,
        USINT,
        UINT,
        UDINT,
        ULINT,
        STRING,
        SHORTSTRING,
        BYTE,
        WORD,
        DWORD,
        LWORD,
        REAL,
        LREAL
    };

    enum class CommonPacketItemIdNumbers {
        Null = 0,
        ListIdentityResponse = 12,
        ConnectionBased = 161,
        ConnectedDataItem = 177,
        UnconnectedDataItem = 178,
        ListServicesResponse = 256,
        SocketAddressInfoO2T = 32768,
        SocketAddressInfoT2O = 32769,
        SequencedAddressItem = 32770
    };

    enum class EIPNetworkStatus {
        OnLine,
        OnLineReadRejected,
        OnLineWriteRejected,
        OnLineForwardOpenReject,
        OffLine
    };

    enum class EncapsulationCommands {
        Nop = 0,
        ListServices = 4,
        ListIdentity = 99,
        ListInterfaces = 100,
        RegisterSession = 101,
        UnRegisterSession = 102,
        SendRRDate = 111,
        SendUintData = 112,
        IndicateStatus = 114,
        Cancel = 115
    };

    std::vector<uint16_t> encapsulationCommands{0, 4, 99, 100, 101, 102, 111, 112, 114, 115};

    enum class EncapsulationStatus {
        Success = 0,
        UnsupportedCommand = 1,
        InsufficientMemory = 2,
        IncorrectData = 3,
        InvalidSessionHandle = 100,
        InvalidLength = 101,
        UnsupportedProtocolRevision = 105
    };

    enum class IdentityObjectState {
        NonExistent = 0,
        DeviceSelfTesting = 1,
        Standby = 2,
        Operational = 3,
        MajorRecoverableFault = 4,
        MajorUnrecoverableFault = 5,
        Default = 255
    };

    enum class TransportClassTriggerAttribute {
        Cyclic = 0,
        ChangeOfState = 16,
        ApplicationObject = 32
    };
}

#endif //ETHERNETIP_EIPENUM_H
