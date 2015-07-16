#import <SystemConfiguration/SCNetworkReachability.h>

#import "MVAvailability.h"
#import "MVChatString.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, MVChatConnectionType) {
	MVChatConnectionUnsupportedType = 0,
	MVChatConnectionICBType = 'icbC',
	MVChatConnectionIRCType = 'ircC',
	MVChatConnectionSILCType = 'silC',
	MVChatConnectionXMPPType = 'xmpC'
};

typedef NS_ENUM(NSInteger, MVChatConnectionStatus) {
	MVChatConnectionDisconnectedStatus = 'disC',
	MVChatConnectionServerDisconnectedStatus = 'sdsC',
	MVChatConnectionConnectingStatus = 'conG',
	MVChatConnectionConnectedStatus = 'conD',
	MVChatConnectionSuspendedStatus = 'susP'
};

typedef NS_ENUM(NSInteger, MVChatConnectionProxy) {
	MVChatConnectionNoProxy = 'nonE',
	MVChatConnectionHTTPProxy = 'httP',
	MVChatConnectionHTTPSProxy = 'htpS',
	MVChatConnectionSOCKS4Proxy = 'soK4',
	MVChatConnectionSOCKS5Proxy = 'soK5'
};

typedef NS_ENUM(NSInteger, MVChatConnectionBouncer) {
	MVChatConnectionNoBouncer = 'nonB',
	MVChatConnectionGenericBouncer = 'gbnC',
	MVChatConnectionColloquyBouncer = 'cbnC'
};

typedef NS_ENUM(NSInteger, MVChatConnectionPublicKeyType) {
	MVChatConnectionServerPublicKeyType = 'serV',
	MVChatConnectionClientPublicKeyType = 'clnT'
};

typedef NS_ENUM(NSInteger, MVChatMessageFormat) {
	MVChatConnectionDefaultMessageFormat = 'cDtF',
	MVChatNoMessageFormat = 'nOcF',
	MVChatWindowsIRCMessageFormat = 'mIrF',
	MVChatCTCPTwoMessageFormat = 'ct2F'
};

typedef NS_ENUM(NSInteger, MVChatConnectionError) {
	MVChatConnectionUnknownError = -1,
	MVChatConnectionNoSuchUserError = -2,
	MVChatConnectionNoSuchRoomError = -3,
	MVChatConnectionNoSuchServerError = -4,
	MVChatConnectionCantSendToRoomError = -5,
	MVChatConnectionCantChangeNickError = -6,
	MVChatConnectionNotInRoomError = -7,
	MVChatConnectionUserNotInRoomError = -8,
	MVChatConnectionRoomIsFullError = -9,
	MVChatConnectionInviteOnlyRoomError = -10,
	MVChatConnectionRoomPasswordIncorrectError = -11,
	MVChatConnectionBannedFromRoomError = -12,
	MVChatConnectionUnknownCommandError = -13,
	MVChatConnectionErroneusNicknameError = -14,
	MVChatConnectionBannedFromServerError = -15,
	MVChatConnectionServerPasswordIncorrectError = -16,
	MVChatConnectionProtocolError = -17,
	MVChatConnectionOutOfBricksError = -18,
	MVChatConnectionCantChangeUsedNickError = -19,
	MVChatConnectionIdentifyToJoinRoomError = -20,
	MVChatConnectionRoomDoesNotSupportModesError = -21,
	MVChatConnectionNickChangedByServicesError = -22,
	MVChatConnectionServicesDownError = -23,
	MVChatConnectionTLSError = -24
};

typedef BOOL (^MVPeerTrustHandler)(SecTrustRef);

@class MVChatRoom;
@class MVChatUser;
@class MVChatUserWatchRule;
@class MVUploadFileTransfer;

extern NSString *MVChatConnectionWatchFeature;

// IRC3v1 Required
extern NSString *MVChatConnectionSASLFeature;
extern NSString *MVChatConnectionMultipleNicknamePrefixFeature;

// IRC3v1 Optional
extern NSString *MVChatConnectionAccountNotify;
extern NSString *MVChatConnectionAwayNotify;
extern NSString *MVChatConnectionExtendedJoin;
extern NSString *MVChatConnectionTLS;

// IRC3v2 Required
extern NSString *MVChatConnectionMessageTags;
extern NSString *MVChatConnectionMessageIntents;
extern NSString *MVChatConnectionMetadata;
extern NSString *MVChatConnectionMonitor;

// IRC3v2 Optional
extern NSString *MVChatConnectionServerTime;
extern NSString *MVChatConnectionBatch;
extern NSString *MVChatConnectionUserhostInNames;
extern NSString *MVChatConnectionChghost;

extern NSString *MVChatConnectionAccountTag;
extern NSString *MVChatConnectionCapNotify;
extern NSString *MVChatConnectionInvite;
extern NSString *MVChatConnectionEchoMessage;

// InspIRCd Enhancements
extern NSString *MVChatConnectionNamesx;

// Notifications
extern NSString *MVChatConnectionWillConnectNotification;
extern NSString *MVChatConnectionDidConnectNotification;
extern NSString *MVChatConnectionDidNotConnectNotification;
extern NSString *MVChatConnectionWillDisconnectNotification;
extern NSString *MVChatConnectionDidDisconnectNotification;
extern NSString *MVChatConnectionGotErrorNotification;
extern NSString *MVChatConnectionErrorNotification;

extern NSString *MVChatConnectionNeedTLSPeerTrustFeedbackNotification; // used when connecting
extern NSString *MVChatConnectionNeedNicknamePasswordNotification;
extern NSString *MVChatConnectionNeedServerPasswordNotification;
extern NSString *MVChatConnectionNeedCertificatePasswordNotification;
extern NSString *MVChatConnectionNeedPublicKeyVerificationNotification;

extern NSString *MVChatConnectionGotBeepNotification;
extern NSString *MVChatConnectionGotImportantMessageNotification;
extern NSString *MVChatConnectionGotInformationalMessageNotification;
extern NSString *MVChatConnectionGotRawMessageNotification;
extern NSString *MVChatConnectionGotPrivateMessageNotification;
extern NSString *MVChatConnectionChatRoomListUpdatedNotification;
extern NSString *MVChatConnectionBatchUpdatesWillBeginNotification;
extern NSString *MVChatConnectionBatchUpdatesDidEndNotification;

extern NSString *MVChatConnectionSelfAwayStatusChangedNotification;

extern NSString *MVChatConnectionNicknameAcceptedNotification;
extern NSString *MVChatConnectionNicknameRejectedNotification;

extern NSString *MVChatConnectionDidIdentifyWithServicesNotification;

extern NSString *MVChatConnectionSubcodeRequestNotification;
extern NSString *MVChatConnectionSubcodeReplyNotification;

extern NSString *MVChatConnectionErrorDomain;

@interface MVChatConnection : NSObject {
@protected
	NSMutableSet *_supportedFeatures;
	MVChatConnectionStatus _status;
	MVChatMessageFormat _outgoingChatFormat;
	NSStringEncoding _encoding;

	NSString *_uniqueIdentifier;

	NSString *_npassword;
	NSMutableDictionary *_knownUsers;
	NSMutableDictionary *_knownRooms;
	NSMutableSet *_joinedRooms;
	MVChatUser *_localUser;
	NSMutableDictionary *_roomsCache;
	NSMutableSet *_pendingRoomAdditions;
	NSMutableSet *_pendingRoomUpdates;
	NSMutableSet *_chatUserWatchRules;
	NSDate *_cachedDate;
	MVChatString *_awayMessage;

	NSMutableDictionary *_persistentInformation;
	NSError *_lastError; // socket errors
	NSError *_serverError; // messages from the server

	SCNetworkReachabilityRef _reachability;

	NSDate *_connectedDate;

	MVChatConnectionProxy _proxy;
	NSString *_proxyServer;
	NSString *_proxyUsername;
	NSString *_proxyPassword;
	unsigned short _proxyServerPort;

	MVChatConnectionBouncer _bouncer;
	NSString *_bouncerServer;
	NSString *_bouncerUsername;
	NSString *_bouncerPassword;
	NSString *_bouncerDeviceIdentifier;
	NSString *_bouncerConnectionIdentifier;
	unsigned short _bouncerServerPort;

	BOOL _secure;
	BOOL _connectedSecurely;
	BOOL _requestsSASL;
	BOOL _roomsWaitForIdentification;
	BOOL _roomListDirty;
	BOOL _userDisconnected;

	NSDate *_lastConnectAttempt;
	NSTimer *_reconnectTimer;
	unsigned short _reconnectAttemptCount;

	NSArray *_alternateNicks;
	unsigned short _nextAltNickIndex;
	NSUInteger _hash;
}
+ (BOOL) supportsURLScheme:(NSString *) scheme;
+ (NSArray *) defaultServerPortsForType:(MVChatConnectionType) type;

#pragma mark -

- (instancetype) initWithType:(MVChatConnectionType) type;
- (instancetype) initWithURL:(NSURL *) url;
- (instancetype) initWithServer:(NSString *) server type:(MVChatConnectionType) type port:(unsigned short) port user:(NSString *) nickname;

#pragma mark -

@property(readonly) MVChatConnectionType type;

@property(copy) NSString *uniqueIdentifier;

@property(strong, readonly) NSSet *supportedFeatures;
@property(readonly) const NSStringEncoding *supportedStringEncodings;

@property(strong, readonly) NSError *lastError;
@property(strong, readonly) NSError *serverError;

@property(strong, readonly) NSString *urlScheme;
@property(strong, readonly) NSURL *url;

@property NSStringEncoding encoding;

@property(copy) NSString *realName;

@property(copy) NSString *nickname;
@property(copy) NSString *preferredNickname;

@property(copy) NSArray *alternateNicknames;
@property(strong, readonly) NSString *nextAlternateNickname;

@property(copy, null_resettable) NSString *nicknamePassword;

@property(strong, readonly) NSString *certificateServiceName;
@property(strong, readonly) NSString *certificatePassword;

@property(copy) NSString *password;

@property(copy) NSString *username;

@property(copy) NSString *server;

@property unsigned short serverPort;

@property MVChatMessageFormat outgoingChatFormat;

@property(getter=didConnectSecurely) BOOL connectedSecurely; // IRCv3.1 + `tls` capability can indicate that the *next* connection will be secure, but not our current one
@property(getter=isSecure) BOOL secure;
@property BOOL requestsSASL;
@property BOOL roomsWaitForIdentification;

@property(copy) NSDictionary *persistentInformation;

@property MVChatConnectionProxy proxyType;
@property(copy) NSString *proxyServer;
@property unsigned short proxyServerPort;
@property(copy) NSString *proxyUsername;
@property(copy) NSString *proxyPassword;

@property MVChatConnectionBouncer bouncerType;
@property(copy) NSString *bouncerServer;
@property unsigned short bouncerServerPort;
@property(copy) NSString *bouncerUsername;
@property(copy) NSString *bouncerPassword;
@property(copy) NSString *bouncerDeviceIdentifier;
@property(copy) NSString *bouncerConnectionIdentifier;

@property(strong, readonly) NSSet *knownChatRooms;
@property(strong, readonly) NSSet *joinedChatRooms;
@property(strong, readonly) NSCharacterSet *chatRoomNamePrefixes;

@property(strong, readonly) NSSet *knownChatUsers;
@property(strong, readonly) MVChatUser *localUser;

@property(strong, readonly) NSSet *chatUserWatchRules;

@property(copy, null_resettable) MVChatString *awayStatusMessage;

@property(readonly, getter=isConnected) BOOL connected;
@property(strong, readonly) NSDate *connectedDate;
@property(strong, readonly) NSDate *nextReconnectAttemptDate;
@property(readonly, getter=isWaitingToReconnect) BOOL waitingToReconnect;
@property(readonly) unsigned short reconnectAttemptCount;
@property(readonly) MVChatConnectionStatus status;
@property(readonly) NSUInteger lag;

#pragma mark -

- (id) persistentInformationObjectForKey:(id) key;
- (void) removePersistentInformationObjectForKey:(id) key;
- (void) setPersistentInformationObject:(id) object forKey:(id) key;

#pragma mark -

- (unsigned short) reconnectAttemptCount;
- (NSDate *) connectedDate;
- (NSDate *) nextReconnectAttemptDate;

#pragma mark -

- (BOOL) supportsFeature:(NSString *) key;
- (BOOL) supportsStringEncoding:(NSStringEncoding) encoding;

#pragma mark -

- (void) connect;
- (void) connectToServer:(NSString *) server onPort:(unsigned short) port asUser:(NSString *) nickname;
- (void) disconnect;
- (void) disconnectWithReason:(MVChatString * __nullable) reason;
- (void) forceDisconnect;

#pragma mark -

- (BOOL) authenticateCertificateWithPassword:(NSString *) password;
- (void) publicKeyVerified:(NSDictionary *) dictionary andAccepted:(BOOL) accepted andAlwaysAccept:(BOOL) alwaysAccept;

#pragma mark -

- (void) sendCommand:(NSString *) command withArguments:(MVChatString * __nullable) arguments;

#pragma mark -

- (void) sendRawMessage:(id) raw;
- (void) sendRawMessage:(id) raw immediately:(BOOL) now;
- (void) sendRawMessageWithFormat:(NSString *) format, ...;
- (void) sendRawMessageImmediatelyWithFormat:(NSString *) format, ...;
- (void) sendRawMessageWithComponents:(id) firstComponent, ...;
- (void) sendRawMessageImmediatelyWithComponents:(id) firstComponent, ...;

#pragma mark -

- (void) processIncomingMessage:(id) raw;
- (void) processIncomingMessage:(id) raw fromServer:(BOOL) fromServer;

#pragma mark -

- (void) joinChatRoomsNamed:(NSArray *) rooms;
- (void) joinChatRoomNamed:(NSString *) room;
- (void) joinChatRoomNamed:(NSString *) room withPassphrase:(NSString * __nullable) passphrase;

#pragma mark -

- (MVChatRoom *) joinedChatRoomWithUniqueIdentifier:(id) identifier;
- (MVChatRoom *) joinedChatRoomWithName:(NSString *) room;

#pragma mark -

- (MVChatRoom *) chatRoomWithUniqueIdentifier:(id) identifier;
- (MVChatRoom *) chatRoomWithName:(NSString *) room;

#pragma mark -

- (NSString *) properNameForChatRoomNamed:(NSString *) room;
- (NSString *) displayNameForChatRoomNamed:(NSString *) room;

#pragma mark -

- (NSSet *) chatUsersWithNickname:(NSString *) nickname;
- (NSSet *) chatUsersWithFingerprint:(NSString *) fingerprint;
- (MVChatUser *) chatUserWithUniqueIdentifier:(id) identifier;

#pragma mark -

- (void) addChatUserWatchRule:(MVChatUserWatchRule *) rule;
- (void) removeChatUserWatchRule:(MVChatUserWatchRule *) rule;

#pragma mark -

- (void) fetchChatRoomList;
- (void) stopFetchingChatRoomList;
@property (readonly, copy) NSMutableDictionary *chatRoomListResults;

#pragma mark -

- (void) scheduleReconnectAttempt;
- (void) cancelPendingReconnectAttempts;

#pragma mark -

- (void) purgeCaches;
@end

#pragma mark -

#if ENABLE(SCRIPTING)
@interface MVChatConnection (MVChatConnectionScripting)
@property(readonly) NSNumber *scriptUniqueIdentifier;
@end
#endif

#pragma mark -

@interface NSObject (MVChatPluginConnectionSupport)
- (void) processIncomingMessageAsData:(NSMutableData *) message from:(MVChatUser *) sender to:(id) receiver attributes:(NSMutableDictionary *)msgAttributes;
- (void) processOutgoingMessageAsData:(NSMutableData *) message to:(id) receiver attributes:(NSDictionary *)msgAttributes;

- (void) processTopicAsData:(NSMutableData *) topic inRoom:(MVChatRoom *) room author:(MVChatUser *) author;

- (BOOL) processSubcodeRequest:(NSString *) command withArguments:(NSData *) arguments fromUser:(MVChatUser *) user;
- (BOOL) processSubcodeReply:(NSString *) command withArguments:(NSData *) arguments fromUser:(MVChatUser *) user;

- (void) connected:(MVChatConnection *) connection;
- (void) disconnecting:(MVChatConnection *) connection;
@end

NS_ASSUME_NONNULL_END
