#pragma once
#include "SDKClasses.h"
#include "Memory.h"
#include "bitbuf.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <xstring>
#include <string>
#include <cstdio>

class INetMessage;
class INetChannelHandler;
class INetChannelInfo
{
public:

	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopbackWrong(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOutWrong(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback
	bool IsLoopback()
	{
		return Memory::CallVirtual<bool(__thiscall*)(void*)>(this, 6)(this);
	}
	bool IsTimingOut()
	{
		return Memory::CallVirtual<bool(__thiscall*)(void*)>(this, 7)(this);
	}
	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetTotalPackets(int flow) const = 0;
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation, float* pflFrameStartTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};
class INetChannel : public INetChannelInfo
{
public:
	virtual ~INetChannel(void) {};

	virtual void SetDataRate(float rate) = 0;
	virtual bool RegisterMessage(INetMessage* msg) = 0;
	virtual bool StartStreaming(unsigned int challengeNr) = 0;
	virtual void ResetStreaming(void) = 0;
	virtual void SetTimeout(float seconds) = 0;
	// virtual void SetDemoRecorder(IDemoRecorder *recorder) = 0;
	virtual void SetChallengeNr(unsigned int chnr) = 0;

	virtual void Reset(void) = 0;
	virtual void Clear(void) = 0;
	virtual void Shutdown(const char* reason) = 0;

	virtual void ProcessPlayback(void) = 0;
	virtual bool ProcessStream(void) = 0;
	virtual void ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;

	//virtual bool SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) = 0;
#ifdef POSIX
	//FORCEINLINE bool SendNetMsg(INetMessage const& msg, bool bForceReliable = false, bool bVoice = false)
	//{
	//	return SendNetMsg(*((INetMessage*)&msg), bForceReliable, bVoice);
	//}
#endif
	// virtual bool SendData(bf_write &msg, bool bReliable = true) = 0;
	virtual bool SendFile(const char* filename, unsigned int transferID) = 0;
	virtual void DenyFile(const char* filename, unsigned int transferID) = 0;
	virtual void RequestFile_OLD(const char* filename,
		unsigned int transferID) = 0; // get rid of this function when we version the
	//virtual void SetChoked(void) = 0;
	// virtual int SendDatagram(bf_write *data) = 0;
	virtual bool Transmit(bool onlyReliable = false) = 0;

	virtual const netadr_t& GetRemoteAddress(void) const = 0;
	virtual INetChannelHandler* GetMsgHandler(void) const = 0;
	virtual int GetDropNumber(void) const = 0;
	virtual int GetSocket(void) const = 0;
	virtual unsigned int GetChallengeNr(void) const = 0;
	virtual void GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
	virtual void SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

	virtual void UpdateMessageStats(int msggroup, int bits) = 0;
	//virtual bool CanPacket(void) const = 0;
	bool CanPacket()
	{
		return Memory::CallVirtual<bool(__thiscall*)(void*)>(this, 53)(this);
	}
	virtual bool IsOverflowed(void) const = 0;
	virtual bool IsTimedOut(void) const = 0;
	virtual bool HasPendingReliableData(void) = 0;

	virtual void SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int RequestFile(const char* filename) = 0;
	virtual float GetTimeSinceLastReceived(void) const = 0; // get time since last received packet in seconds

	virtual void SetMaxBufferSize(bool bReliable, int nBYTEs, bool bVoice = false) = 0;

	virtual bool IsNull() const = 0;
	virtual int GetNumBitsWritten(bool bReliable) = 0;
	virtual void SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;

	// Max # of payload BYTEs before we must split/fragment the packet
	virtual void SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int GetMaxRoutablePayloadSize() = 0;

	virtual int GetProtocolVersion() = 0;

public:
	// bool __fastcall Hooked_SendNetMessage(CNetChannel* pNet, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
	bool SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) {
		return Memory::CallVirtual<bool(__thiscall*)(void*, INetMessage&, bool, bool)>(this, 37)(this, msg, bForceReliable, bVoice);
	}
	void SetChoked() {
		return Memory::CallVirtual<void(__thiscall*)(void*)>(this, 42)(this);
	}
	int SendDatagram(bf_write* write);
};
class INetMessage
{
public:
	virtual ~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;             // set to true if it's a reliable message

	virtual bool Process(void) = 0; // calles the recently set handler to process this message

	virtual bool ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual bool WriteToBuffer(bf_write& buffer) = 0; // returns true if writing was OK

	virtual bool IsReliable(void) const = 0; // true, if message needs reliable handling

	virtual int GetType(void) const = 0;         // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;        // returns net message group of this message
	virtual const char* GetName(void) const = 0; // returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
};
typedef enum class FileHandle_t;
template<class T>
class CUtlMemory
{
public: /* fuck you*/
	typedef T ElemType_t;
};
#define MAX_SUBCHANNELS		8		// we have 8 alternative send&wait bits
class CNetChannel : public INetChannel
{

public: // netchan structurs
	typedef struct dataFragments_s
	{
		FileHandle_t file;                 // open file handle
		char         filename[MAX_OSPATH]; // filename
		char* buffer;               // if NULL it's a file
		unsigned int BYTEs;                // size in BYTEs
		unsigned int bits;                 // size in bits
		unsigned int transferID;           // only for files
		bool         isCompressed;         // true if data is bzip compressed
		unsigned int nUncompressedSize;    // full size in BYTEs
		bool         asTCP;                // send as TCP stream
		int          numFragments;         // number of total fragments
		int          ackedFragments;       // number of fragments send & acknowledged
		int          pendingFragments;     // number of fragments send, but not acknowledged yet
	} dataFragments_t;

	struct subChannel_s
	{
		int startFraggment[MAX_STREAMS];
		int numFragments[MAX_STREAMS];
		int sendSeqNr;
		int state; // 0 = free, 1 = scheduled to send, 2 = send & waiting, 3 = dirty
		int index; // index in m_SubChannels[]

		void Free()
		{
			state = SUBCHANNEL_FREE;
			sendSeqNr = -1;
			for (int i = 0; i < MAX_STREAMS; i++)
			{
				numFragments[i] = 0;
				startFraggment[i] = -1;
			}
		}
	};

	typedef struct netframe_s
	{
		float          time;        // net_time received/send
		int            size;        // total size in BYTEs
		float          latency;     // raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
		float          avg_latency; // averaged ping for this packet
		bool           valid;       // false if dropped, lost, flushed
		int            choked;      // number of previously chocked packets
		int            dropped;
		float          m_flInterpolationAmount;
		unsigned short msggroups[INetChannelInfo::TOTAL]; // received BYTEs for each message group
	} netframe_t;
	typedef struct
	{
		float       nextcompute;      // Time when we should recompute k/sec data
		float       avgBYTEspersec;   // average BYTEs/sec
		float       avgpacketspersec; // average packets/sec
		float       avgloss;          // average packet loss [0..1]
		float       avgchoke;         // average packet choke [0..1]
		float       avglatency;       // average ping, not cleaned
		float       latency;          // current ping, more accurate also more jittering
		int         totalpackets;     // total processed packets
		int         totalBYTEs;       // total processed BYTEs
		int         currentindex;     // current frame index
		netframe_t  frames[64];       // frame history
		netframe_t* currentframe;     // current frame
	} netflow_t;
	typedef struct netpacket_s
	{
		netadr_t		from;		// sender IP
		int				source;		// received source 
		double			received;	// received time
		unsigned char* data;		// pointer to raw packet data
		bf_read			message;	// easy bitbuf data access
		int				size;		// size in bytes
		int				wiresize;   // size in bytes before decompression
		bool			stream;		// was send as stream
		struct netpacket_s* pNext;	// for internal use, should be NULL in public
	} netpacket_t;

public:
	bool ProcessingMessages;
	bool ShouldDelete;

	int OutSequenceNr;
	int InSequenceNr;
	int OutSequenceNrAck;
	int OutReliableState;
	int InReliableState;
	int ChokedPackets;

	// Reliable data buffer, send which each packet (or put in waiting list)
	bf_write	m_StreamReliable;
	CUtlMemory<byte> m_ReliableDataBuffer;

	// unreliable message buffer, cleared which each packet
	bf_write	m_StreamUnreliable;
	CUtlMemory<byte> m_UnreliableDataBuffer;

	bf_write	m_StreamVoice;
	CUtlMemory<byte> m_VoiceDataBuffer;

	int			m_Socket;   // NS_SERVER or NS_CLIENT index, depending on channel.
	int			m_StreamSocket;	// TCP socket handle

	unsigned int m_MaxReliablePayloadSize;	// max size of reliable payload in a single packet	

	// Address this channel is talking to.
	netadr_t	remote_address;

	// For timeouts.  Time last message was received.
	float		last_received;
	// Time when channel was connected.
	double      connect_time;

	// Bandwidth choke
	// Bytes per second
	int			m_Rate;
	// If realtime > cleartime, free to send next packet
	double		m_fClearTime;

	//CUtlVector<dataFragments_t*>	m_WaitingList[MAX_STREAMS];	// waiting list for reliable data and file transfer
	dataFragments_t					m_ReceiveList[MAX_STREAMS]; // receive buffers for streams
	subChannel_s					m_SubChannels[MAX_SUBCHANNELS];

	unsigned int	m_FileRequestCounter;	// increasing counter with each file request
	bool			m_bFileBackgroundTranmission; // if true, only send 1 fragment per packet
	bool			m_bUseCompression;	// if true, larger reliable data will be bzip compressed

	// TCP stream state maschine:
	bool		m_StreamActive;		// true if TCP is active
	int			m_SteamType;		// STREAM_CMD_*
	int			m_StreamSeqNr;		// each blob send of TCP as an increasing ID
	int			m_StreamLength;		// total length of current stream blob
	int			m_StreamReceived;	// length of already received bytes
	char		m_SteamFile[MAX_OSPATH];	// if receiving file, this is it's name
	CUtlMemory<byte> m_StreamData;			// Here goes the stream data (if not file). Only allocated if we're going to use it.

	// packet history
	netflow_t		m_DataFlow[MAX_FLOWS];

	int				m_MsgStats[INetChannelInfo::TOTAL];	// total bytes for each message group


	int				m_PacketDrop;	// packets lost before getting last update (was global net_drop)

	char			m_Name[32];		// channel name

	unsigned int	m_ChallengeNr;	// unique, random challenge number 

	float		m_Timeout;		// in seconds 

	INetChannelHandler* m_MessageHandler;	// who registers and processes messages
	//CUtlVector<INetMessage*>	m_NetMessages;		// list of registered message
	void* m_DemoRecorder;			// if != NULL points to a recording/playback demo object
	int							m_nQueuedPackets;

	float						m_flInterpolationAmount;
	float						m_flRemoteFrameTime;
	float						m_flRemoteFrameTimeStdDeviation;
	int							m_nMaxRoutablePayloadSize;

	int							m_nSplitPacketSequence;
	bool						m_bStreamContainsChallenge;  // true if PACKET_FLAG_CHALLENGE was set when receiving packets from the sender

	int							m_nProtocolVersion;
};



class CNetMessage : public INetMessage {
public:
	CNetMessage() {
		m_bReliable = true;
		m_NetChannel = nullptr;
	}

	virtual ~CNetMessage() {};

	virtual int GetGroup() const { return INetChannelInfo::GENERIC; }
	INetChannel* GetNetChannel() const { return m_NetChannel; }

	virtual void SetReliable(bool state) { m_bReliable = state; };
	virtual bool IsReliable() const { return m_bReliable; };
	virtual void SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual bool Process() { return false; }; // no handler set

protected:
	bool m_bReliable;          // true if message should be send reliable
	INetChannel* m_NetChannel; // netchannel this message is from/for
};
#define NUM_NEW_COMMAND_BITS		4
#define MAX_NEW_COMMANDS			((1 << NUM_NEW_COMMAND_BITS)-1)
#define NUM_BACKUP_COMMAND_BITS		3
#define MAX_BACKUP_COMMANDS			((1 << NUM_BACKUP_COMMAND_BITS)-1)
#define NETMSG_TYPE_BITS	6	// must be 2^NETMSG_TYPE_BITS > SVC_LASTMSG
#define clc_Move 9 
class CLC_Move : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer)
	{
		m_nNewCommands = buffer.ReadUBitLong(NUM_NEW_COMMAND_BITS);
		m_nBackupCommands = buffer.ReadUBitLong(NUM_BACKUP_COMMAND_BITS);
		m_nLength = buffer.ReadWord();
		m_DataIn = buffer;
		return buffer.SeekRelative(m_nLength);
	}

	bool WriteToBuffer(bf_write& buffer)
	{
		buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
		m_nLength = m_DataOut.GetNumBitsWritten();

		buffer.WriteUBitLong(m_nNewCommands, NUM_NEW_COMMAND_BITS);
		buffer.WriteUBitLong(m_nBackupCommands, NUM_BACKUP_COMMAND_BITS);

		buffer.WriteWord(m_nLength);

		return buffer.WriteBits(m_DataOut.GetData(), m_nLength);
	}

	const char* ToString() const { return "CLC_Move"; }
	int GetType() const { return clc_Move; }
	const char* GetName() const { return "clc_Move"; }
	void* m_pMessageHandler;
	int GetGroup() const { return INetChannelInfo::MOVE; }
	CLC_Move() { m_bReliable = false; }


public:
	int m_nBackupCommands;
	int m_nNewCommands;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
};
#define NET_TICK_SCALEUP	100000.0f
class NET_Tick : public CNetMessage
{
public:
	bool WriteToBuffer(bf_write& buffer)
	{
		buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
		buffer.WriteLong(m_nTick);
		buffer.WriteUBitLong(std::clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTime), 0, 65535), 16);
		buffer.WriteUBitLong(std::clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTimeStdDeviation), 0, 65535), 16);
		return !buffer.IsOverflowed();
	}

	bool ReadFromBuffer(bf_read& buffer)
	{
		m_nTick = buffer.ReadLong();
		m_flHostFrameTime = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
		m_flHostFrameTimeStdDeviation = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
		return !buffer.IsOverflowed();
	}

	const char* ToString(void) const { return "NET_Tick"; }
	int GetType() const { return 3; } // net_Tick is 3, you fucking moron.
	const char* GetName() const { return "net_Tick"; }

	NET_Tick()
	{
		m_bReliable = false;
		m_flHostFrameTime = 0;
		m_flHostFrameTimeStdDeviation = 0;
	};

	NET_Tick(int tick, float hostFrametime, float hostFrametime_stddeviation)
	{
		m_bReliable = false;
		m_nTick = tick;
		m_flHostFrameTime = hostFrametime;
		m_flHostFrameTimeStdDeviation = hostFrametime_stddeviation;
	};

public:
	int			m_nTick = 0;
	float		m_flHostFrameTime = 0;
	float		m_flHostFrameTimeStdDeviation = 0;
};
class CClockDriftMgr
{
public:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int m_iCurClockOffset;

	int m_nServerTick;		// Last-received tick from the server.
	int	m_nClientTick;		// The client's own tick counter (specifically, for interpolation during rendering).
							// The server may be on a slightly different tick and the client will drift towards it.
};
class CEventInfo
{
public:
	enum
	{
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,  // ( 1<<8 bits == 256, but only using 192 below )
	};

	~CEventInfo()
	{
		if (pData)
		{
			delete pData;
		}
	}

	// 0 implies not in use
	short classID;

	// If non-zero, the delay time when the event should be fired ( fixed up on the client )
	float fire_delay;

	// send table pointer or NULL if send as full update
	const RecvTable* pSendTable; // idk if this is the right class kek
	const ClientClass* pClientClass;

	// Length of data bits
	int		bits;
	// Raw event data
	byte* pData;
	// CLIENT ONLY Reliable or not, etc.
	int		flags;
private:
	char pad[24];
public:
	CEventInfo* next;
};
#define net_SignonState 6 // signals current signon state
#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24
#define SIGNONSTATE_FULL		6	// we are fully connected, first non-delta packet received
#define SIGNONSTATE_CONNECTED	2	// client is connected to server, netchans ready


class CClientState
{

public:
	inline bool IsActive(void) const { return m_nSignonState == SIGNONSTATE_FULL; }
	inline bool IsConnected(void) const { return m_nSignonState >= SIGNONSTATE_CONNECTED; }

	bool IsPaused();
public:
	byte pad0[0x10];
	CNetChannel* m_NetChannel;
	byte pad1[0x11C];
	int m_nSignonState;
	byte pad2[0x4];
	double m_flNextCmdTime;
	byte pad3[0x4];
	int m_nServerCount;
	unsigned __int64 m_ulGameServerSteamID;
	int m_nCurrentSequence;
	CClockDriftMgr_pad m_ClockDriftMgr;
	int m_nDeltaTick;
	byte pad4[0x218];
	int m_nMaxClients;
	byte pad5[0x4868];
	float m_frameTime;
	int lastoutgoingcommand;
	int chokedcommands;
	int last_command_ack;
};