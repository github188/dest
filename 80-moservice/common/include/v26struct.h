#pragma once
#include "vccommon.h"
//#include "mauutility.h"
#include "lanman.h"

#ifdef WIN32
#pragma comment( lib, "ws2_32.lib" ) 
#pragma pack( push )
#pragma pack( 1 )
#define window( x )	x
#define PACKED 
#else
#include <netinet/in.h>
#define window( x )
#if defined(__ETI_linux__)
#define PACKED
#else
#define PACKED __attribute__((__packed__))	// ȡ�����������Ż�����
#endif
#endif

// 2.6�汾�Ľṹ�嶨���������namespace���°汾�Ľṹ�����
namespace v26
{
	// ����ȫ������const���¶��壬��겻��namespace����
	// ��ֹ�����汾�޸��˺�������ϰ汾�ṹ�巢���仯
	const u8 maxlen_confname = 64;
	const u8 maxlen_e164 = 16;
	const u8 maxlen_guid = 32;
	const u8 maxlen_domainname = 64;
	const u8 maxlen_moid = 36;
	const u8 maxlen_alias = 128;
	const u8 maxlen_shortno = 6;
	const u8 maxlen_reserver = 32;
	const u8 maxlen_pwd = 32;
	const u8 max_vmp_mem = 16; 

	//2.6�Ǳ���Ϣ����Ŷ���
	const u16 kdvp_nsm_success = 0;
	const u16 kdvp_nsm_ser_db_err = 1;
	const u16 kdvp_nsm_timeout_err = 2;				 //ƽ̨�ڲ���ʱ
	const u16 kdvp_nsm_note164_err = 3;
	const u16 kdvp_nsm_data_err = 4;
	const u16 kdvp_nsm_mcu_noidlevpu = 5;			 //û���㹻�����Ŀ���ý�崦��
	const u16 kdvp_nsm_mcu_minimediaconfmtnum = 6;   //����С��ý���������ն�����, 
	const u16 kdvp_nsm_mcu_maxconfnum = 7;           //�Ѵ���������
	const u16 kdvp_nsm_mcu_encrypeerr = 8;           //��������ģʽ����
	const u16 kdvp_nsm_mcu_confnotexist = 9;         //���鲻����
	const u16 kdvp_nsm_mcu_maxmtnum = 10;            //�ﵽ���λ���Ա��
	const u16 kdvp_nsm_gk_mpcd_disconnected = 11;    //MPCD��GK����          nu����
	const u16 kdvp_nsm_mcu_confresuming = 12;        //MCU�����ָ���
	const u16 kdvp_nsm_mcu_passerr = 13;             //�����������
	const u16 kdvp_nsm_mcu_nsatpms = 14;             //������Ȩ��
	const u16 kdvp_nsm_mcu_nsatdaddr = 15;           //û���鲥��ַ
	const u16 kdvp_nsm_mcu_confe164repeat = 16;      //����E164���ظ�
	const u16 kdvp_nsm_mcu_callernum_exceed = 17;
	const u16 kdvp_nsm_mcu_undefined = 18;
	const u16 kdvp_nsm_mcu_peer_nodisturb = 19;       //��������žܾ�

	//�»��������Ϣ
	struct TConfBaseInfo
	{
		s8  m_achConfName[maxlen_confname + 1];
		s8  m_achConfE164[maxlen_e164 + 1];
		s8	m_achDomainGUID[maxlen_guid + 1];		     //guid
		s8	m_achDomainName[maxlen_domainname + 1];		 //����
		s8  m_achDomainMOID[maxlen_moid + 1];		     //���MOID

		TConfBaseInfo()
		{
			SetNull();
		}

		void SetNull(void)
		{
			memset(&m_achConfName, 0, sizeof(m_achConfName));
			memset(&m_achConfE164, 0, sizeof(m_achConfE164));
			memset(&m_achDomainGUID, 0, sizeof(m_achDomainGUID));
			memset(&m_achDomainName, 0, sizeof(m_achDomainName));
			memset(&m_achDomainMOID, 0, sizeof(m_achDomainMOID));
		}

		//������
		void SetConfName(LPCSTR lpszConfName)
		{
			if (lpszConfName != NULL)
			{
				strncpy(m_achConfName, lpszConfName, sizeof(m_achConfName));
				m_achConfName[sizeof(m_achConfName) - 1] = '\0';
			}
			else
			{
				memset(m_achConfName, 0, sizeof(m_achConfName));
			}
		}
		LPCSTR GetConfName(void) const
		{
			return m_achConfName;
		}

		//����E164
		void SetConfE164(LPCSTR lpszConfE164)
		{
			if (lpszConfE164 != NULL)
			{
				strncpy(m_achConfE164, lpszConfE164, sizeof(m_achConfE164));
				m_achConfE164[sizeof(m_achConfE164) - 1] = '\0';
			}
			else
			{
				memset(m_achConfE164, 0, sizeof(m_achConfE164));
			}
		}
		LPCSTR GetConfE164(void) const
		{
			return m_achConfE164;
		}

		//�����������GUID
		void SetDomainGUID(LPCSTR lpszDomainGUID)
		{
			if (lpszDomainGUID != NULL)
			{
				strncpy(m_achDomainGUID, lpszDomainGUID, sizeof(m_achDomainGUID));
				m_achDomainGUID[sizeof(m_achDomainGUID) - 1] = '\0';
			}
			else
			{
				memset(m_achDomainGUID, 0, sizeof(m_achDomainGUID));
			}
		}
		LPCSTR GetDomainGUID(void) const
		{
			return m_achDomainGUID;
		}

		//���������������
		void  SetDomainName(LPCSTR lpszDomainName)
		{
			if (lpszDomainName != NULL)
			{
				strncpy(m_achDomainName, lpszDomainName, sizeof(m_achDomainName));
				m_achDomainName[sizeof(m_achDomainName) - 1] = '\0';
			}
			else
			{
				memset(m_achDomainName, 0, sizeof(m_achDomainName));
			}
		}
		LPCSTR GetDomainName(void) const
		{
			return m_achDomainName;
		}

		//DomainMOID
		void SetDomainMOID(const s8 *pchDomainMOID)
		{
			memset(m_achDomainMOID, 0, sizeof(m_achDomainMOID));
			if (NULL != pchDomainMOID)
			{
				strncpy(m_achDomainMOID, pchDomainMOID, sizeof(m_achDomainMOID) - 1);
			}
			return;
		}
		const s8* GetDomainMOID(void) const
		{
			return m_achDomainMOID;
		}

		//��ȡ��������к� #����λ����
		u32 GetConfSerialCode(void) const
		{
			u32 dwSerialCode = 0;
			if (strlen(m_achConfE164) != 13)
			{
				return dwSerialCode;
			}
			s8 * pchSerial = (s8 *)m_achConfE164 + strlen(m_achConfE164) - 6;
			dwSerialCode = atol(pchSerial);
			return dwSerialCode;
		}

		void Print(void)
		{
			OspPrintf(TRUE, FALSE, "\n\n");
			OspPrintf(TRUE, FALSE, "Conf Name is %s\n", GetConfName());
			OspPrintf(TRUE, FALSE, "Conf E164 is %s\n", GetConfE164());
			OspPrintf(TRUE, FALSE, "Conf DomainGUID is %s\n", GetDomainGUID());
			OspPrintf(TRUE, FALSE, "Conf DomainName is %s\n", GetDomainName());
			OspPrintf(TRUE, FALSE, "Conf MoID is %s\n", GetDomainMOID());
		}


		BOOL32 operator <(const TConfBaseInfo & other) const
		{
			if (strncmp(m_achConfE164, other.GetConfE164(), sizeof(m_achConfE164)) < 0)
			{
				return TRUE;
			}
			return FALSE;
		}
	}
	PACKED
		;

	//������ϸ��Ϣ�ṹ
	struct TConfDetailInfo
	{
		TConfBaseInfo m_tNewConfBaseInfo;			//������Ϣ

		u8        m_byConfStyle;						//1-�ٿ��л��飬2-���л���, 3-ԤԼ����, 4-ģ��.�磺CONF_STYLE_BOOK
		s8		  m_achShortNo[maxlen_shortno + 1];      //����̺ţ�6λ��
		u8        m_byEncryptMode;						//�������ģʽ: 0-������, 1-des����,2-aes����
		u8        m_byConfMode;							//����ģʽ��0 - ���壬1 - ����, 2 - ���� ,3 - �Զ���
		u16       m_wBitRate;							//��������(��λ:Kbps,1K=1024)
		u8	      m_byResolution;					    //�ֱ��� VIDEO_FORMAT_AUTO, ...

		u8        m_byOpenMode;							//����ģʽ��1-����������� 2-��ȫ����
		u8        m_byDiscussConf;						//�Ƿ����ۻ���: 0-�������ۻ���(�ݽ�����) 1-���ۻ���

		TKdvTime  m_tStartTime;						    //��ʼʱ��
		u16       m_wDuration;							//����ʱ��(����)
		s8        m_achAlias[maxlen_alias + 1];			//�����˱����ַ���
		u8	      m_byDualMode;							//�����˫������ʽ: 0-������ 1-�����ն�

		u8        m_bySatDCastMode;                     //add shenbiao[2013-7-17]�Ƿ�֧�����Ƿ�ɢ���飺0-��֧�֣�1-֧��
		//rnn[2013/11/01]�Ƿ񿪷Ż���������				
		u8		  m_byPublicConf;						//0 -�����ţ� 1 - ����
		//end			
		u8        m_abyReserverd[62];                   //�����ֽ�

		TConfDetailInfo()
		{
			SetNull();
		}

		void SetNull(void)
		{
			m_tNewConfBaseInfo.SetNull();
			m_byConfStyle = 0;

			m_byEncryptMode = 0;
			m_wBitRate = 0;
			m_byResolution = 0;
			m_byOpenMode = CONF_OPENMODE_OPEN;
			m_byDiscussConf = 0;
			m_wDuration = 0;
			m_byDualMode = 0;
			m_bySatDCastMode = 0;
			m_byPublicConf = 0;
			memset(m_achAlias, 0, sizeof(m_achAlias));
			memset(m_achShortNo, 0, sizeof(m_achShortNo));
		}

		//�����˫������ʽ
		void   SetDualMode(u8 byDualMode)
		{
			m_byDualMode = byDualMode;
		}
		u8	   GetDualMode(void) const
		{
			return m_byDualMode;
		}

		//����ģʽ
		void SetConfMode(u8 byConfMode)
		{
			m_byConfMode = byConfMode;
		}
		u8   GetConfMode(void)
		{
			return m_byConfMode;
		}

		//ConfStyle
		void SetConfStyle(u8 byConfStyle)
		{
			m_byConfStyle = byConfStyle;
		}
		u8 GetConfStyle(void) const
		{
			return m_byConfStyle;
		}

		//����ģʽ
		void SetDiscuss(u8 byDiscussConf)
		{
			m_byDiscussConf = byDiscussConf;
		}
		u8 GetDiscuss(void) const
		{
			return m_byDiscussConf;
		}

		//����ģʽ
		void SetOpenMode(u8   byOpenMode)
		{
			m_byOpenMode = byOpenMode;
		}
		u8 GetOpenMode(void) const
		{
			return m_byOpenMode;
		}

		//Duration
		u16 GetDuration() const
		{
			return m_wDuration;
		}
		void SetDuration(u16 wDuration)
		{
			m_wDuration = wDuration;
		}

		//m_tStartTime
		TKdvTime GetStartTime() const
		{
			return m_tStartTime;
		}
		void SetStartTime(TKdvTime tTime)
		{
			m_tStartTime = tTime;
		}

		//m_achAlias
		s8* GetConfCreater()
		{
			return m_achAlias;
		}
		void SetConfCreater(s8* pszCreater)
		{
			memset(m_achAlias, 0, sizeof(m_achAlias));
			if (NULL != pszCreater)
			{
				strncpy(m_achAlias, pszCreater, sizeof(m_achAlias) - 1);
			}
			return;
		}

		//BitRate
		void SetBitRate(u16 wBitRate)
		{
			m_wBitRate = htons(wBitRate);
		}
		u16 GetBitRate(void) const
		{
			return ntohs(m_wBitRate);
		}

		//�ֱ���
		void SetResolution(u8 byRes)
		{
			m_byResolution = byRes;
		}
		u8 GetResolution(void) const
		{
			return m_byResolution;
		}

		//EncryptMode
		u8 GetEncryptMode() const
		{
			return m_byEncryptMode;
		}
		void SetEncryptMode(u8 byMode)
		{
			m_byEncryptMode = byMode;
		}

		void SetSatDCastMode(BOOL32 bSatDCastMode)
		{
			if (bSatDCastMode)
			{
				m_bySatDCastMode = 1;
			}
			else
			{
				m_bySatDCastMode = 0;
			}
		}
		BOOL32 IsSatDCastMode() const
		{
			return m_bySatDCastMode == 0 ? FALSE : TRUE;
		}

		//�����ҿ�������
		void SetPublicConf(u8 byPublicConf)
		{
			m_byPublicConf = byPublicConf;
		}
		u8 GetPublicConf(void) const
		{
			return m_byPublicConf;
		}

		void SetConfShortNO(LPCSTR lpShortNo)
		{
			if (lpShortNo != NULL)
			{
				strncpy(m_achShortNo, lpShortNo, sizeof(m_achShortNo));
				m_achShortNo[sizeof(m_achShortNo) - 1] = '\0';
			}
			else
			{
				memset(m_achShortNo, 0, sizeof(m_achShortNo));
			}
		}
		LPCSTR GetConfShortNO(void) const
		{
			return m_achShortNo;
		}

		void Print(void)
		{
			OspPrintf(TRUE, FALSE, "\n\n");
			m_tNewConfBaseInfo.Print();

			OspPrintf(TRUE, FALSE, "Conf EncryptMode is %d\n", GetEncryptMode());
			OspPrintf(TRUE, FALSE, "Conf BitRate is %d\n", GetBitRate());
			OspPrintf(TRUE, FALSE, "Conf resolusion is %d\n", GetResolution());
			OspPrintf(TRUE, FALSE, "ConfMode is %d\n", GetConfMode());

			OspPrintf(TRUE, FALSE, "Conf Duration  is %d\n", GetDuration());
			OspPrintf(TRUE, FALSE, "Conf StartTime is %d-%d-%d %d:%d:%d\n", m_tStartTime.GetYear(), m_tStartTime.GetMonth(), m_tStartTime.GetDay(),
				m_tStartTime.GetHour(), m_tStartTime.GetMinute(), m_tStartTime.GetSecond());

			OspPrintf(TRUE, FALSE, "Creater alias  is <%s>\n", GetConfCreater());
			OspPrintf(TRUE, FALSE, "Conf DualMode  is <%d>\n", GetDualMode());
			OspPrintf(TRUE, FALSE, "Conf ShortNum is %d \n", GetConfShortNO());

			if (IsSatDCastMode())
			{
				OspPrintf(TRUE, FALSE, "Conf is Stat conf\n");
			}
			else
			{
				OspPrintf(TRUE, FALSE, "Conf is not Stat conf\n");
			}

			OspPrintf(TRUE, FALSE, "Conf Public Attribute is %d\n\n", GetPublicConf());

		}


		BOOL32 operator <(const TConfDetailInfo & other) const
		{
			if (strncmp(m_tNewConfBaseInfo.GetConfE164(), other.m_tNewConfBaseInfo.GetConfE164(), MAXLEN_CONFE164) < 0)
			{
				return TRUE;
			}
			return FALSE;
		}

	}
	PACKED;


	//5.0֮ǰ���������ṹ(len:23)
	struct TCapSupport
	{
	protected:		
		u8          m_bySupportMMcu;        //�Ƿ�֧�ֺϲ�����
		u8          m_bySupportH224;	    //�Ƿ�֧��H224
		u8          m_bySupportT120;	    //�Ƿ�֧��T120  
		u8          m_byEncryptMode;        //����ģʽCONF_ENCRYPTMODE_NONE, CONF_ENCRYPTMODE_DES, CONF_ENCRYPTMODE_AES
		u8          m_byDStreamType;        //VIDEO_DSTREAM_H263PLUS,...

		//˫������
		TDStreamCap   m_tDStreamCap; 

		//ͬʱ������
		TSimCapSet  m_tMainSimCapSet;    //����ͬʱ������
		TSimCapSet  m_tSecondSimCapSet;  //����ͬʱ������

	public:
		//���캯��
		TCapSupport( void ) { Clear(); }

		void    SetSupportMMcu( BOOL32 bMMcu) { m_bySupportMMcu = (bMMcu ? 1:0); }
		BOOL32  IsSupportMMcu() const { return (0 != m_bySupportMMcu); }

		void    SetEncryptMode(u8 byMode) { m_byEncryptMode = byMode; }
		u8      GetEncryptMode(void) const { return m_byEncryptMode; }

		void    SetSupportH224(BOOL32  bSupportH224){ m_bySupportH224 = (bSupportH224 ? 1:0); } 
		BOOL32  IsSupportH224( void ) const { return (0 != m_bySupportH224); }  

		void    SetSupportT120(BOOL32   bSupportT120) { m_bySupportT120 = (bSupportT120 ? 1:0);} 
		BOOL32  IsSupportT120(void) const { return (0 != m_bySupportT120); }   

		u8      GetMainVideoType(void) const { return m_tMainSimCapSet.GetVideoMediaType();    }
		u8      GetMainAudioType(void) const { return m_tMainSimCapSet.GetAudioMediaType();    }
		u8      GetSecVideoType(void) const { return m_tSecondSimCapSet.GetVideoMediaType();   }
		u8      GetSecAudioType(void) const { return m_tSecondSimCapSet.GetAudioMediaType();   }

		void    SetMainVideoResolution(u8 byRes) { m_tMainSimCapSet.SetVideoResolution(byRes);    }
		u8      GetMainVideoResolution(void) const { return m_tMainSimCapSet.GetVideoResolution();    }

		void    SetSecVideoResolution(u8 byRes) { m_tSecondSimCapSet.SetVideoResolution(byRes);    }
		u8      GetSecVideoResolution(void) const { return m_tSecondSimCapSet.GetVideoResolution();    }

		//�ڶ�·��Ƶ��˫��������������
		void    SetDStreamResolution(u8 byRes) { m_tDStreamCap.SetResolution(byRes); }
		u8      GetDStreamResolution(void) const { return m_tDStreamCap.GetResolution(); }

		//֡��
		void    SetMainVidFrmRate(u8 byMPI) { m_tMainSimCapSet.SetVideoFrameRate(byMPI); }
		u8      GetMainVidFrmRate(void) const { return m_tMainSimCapSet.GetVideoFrameRate();    }

		void    SetSecVidFrmRate(u8 byMPI) { m_tSecondSimCapSet.SetVideoFrameRate(byMPI);    }
		u8      GetSecVidFrmRate(void) const { return m_tSecondSimCapSet.GetVideoFrameRate();    }

		void    SetDStreamFrmRate(u8 byMPI) { m_tDStreamCap.SetFrameRate(byMPI);    }
		u8      GetDStreamFrmRate(void) const { return m_tDStreamCap.GetFrameRate();    }

		//H264�Զ���֡��
		void    SetMainVidUsrDefFPS(u8 byFPS) { m_tMainSimCapSet.SetUserDefFrameRate(byFPS);    }
		u8      GetMainVidUsrDefFPS(void) const { return m_tMainSimCapSet.GetUserDefFrameRate();    }
		BOOL32  IsMainVidUsrDefFPS(void) const { return m_tMainSimCapSet.IsFrameRateUserDefined();    }

		void    SetSecVidUsrDefFPS(u8 byFPS) { m_tSecondSimCapSet.SetUserDefFrameRate(byFPS);    }
		u8      GetSecVidUsrDefFPS(void) const { return m_tSecondSimCapSet.GetUserDefFrameRate();    }
		BOOL32  IsSecVidUsrDefFPS(void) const { return m_tSecondSimCapSet.IsFrameRateUserDefined();    }

		void    SetDStreamUsrDefFPS(u8 byFPS) { m_tDStreamCap.SetUserDefFrameRate(byFPS);    }
		u8      GetDStreamUsrDefFPS(void) const { return m_tDStreamCap.GetUserDefFrameRate();    }
		BOOL32  IsDStreamUsrDefFPS(void) const { return m_tDStreamCap.IsFrameRateUserDefined();    }

		//��ʱ����
		void    SetDStreamMaxBitRate(u16 wMaxRate) { m_tDStreamCap.SetMaxBitRate(wMaxRate); }
		u16     GetDStreamMaxBitRate(void) const { return m_tDStreamCap.GetMaxBitRate(); }

		void    SetDStreamFrameRate(u8 byFrameRate) { m_tDStreamCap.SetFrameRate(byFrameRate); }
		u8      GetDStreamFrameRate(void) const { return m_tDStreamCap.GetFrameRate(); }

		void    SetDStreamType( u8 byDStreamType ); //�����ʹ��    
		u8      GetDStreamType( void ) const { return m_byDStreamType; } //�����ʹ��

		void    SetDStreamSupportH239( BOOL32 bSupport ) { m_tDStreamCap.SetSupportH239(bSupport); } //��MCU�ڲ�ʹ��
		BOOL32  IsDStreamSupportH239( void ) const { return m_tDStreamCap.IsSupportH239(); } //�ڶ�·��Ƶ�Ƿ�֧��H239

		void    SetDStreamMediaType( u8 byMediaType ) { m_tDStreamCap.SetMediaType(byMediaType); } //��MCU�ڲ�ʹ��
		u8      GetDStreamMediaType( void ) const { return m_tDStreamCap.GetMediaType(); }	

		TDStreamCap GetDStreamCapSet( void ) const { return m_tDStreamCap; } 

		//��һ·����Ƶ����������
		void    SetMainSimCapSet( TSimCapSet &tSimCapSet ) { memcpy(&m_tMainSimCapSet, &tSimCapSet, sizeof(TSimCapSet)); }    
		TSimCapSet  GetMainSimCapSet( void ) const { return m_tMainSimCapSet; }

		void    SetSecondSimCapSet( TSimCapSet &tSimCapSet ) { memcpy(&m_tSecondSimCapSet, &tSimCapSet, sizeof(TSimCapSet)); }
		TSimCapSet  GetSecondSimCapSet( void ) const { return m_tSecondSimCapSet; }

		BOOL32  IsSupportMediaType( u8 byMediaType ) const; //��һ·����Ƶ�Ƿ�֧��ĳ��ý������	

		void Clear( void )
		{
			m_bySupportMMcu = 0;
			m_bySupportH224 = 0;
			m_bySupportT120 = 0;
			m_byEncryptMode = CONF_ENCRYPTMODE_NONE;
			m_byDStreamType = VIDEO_DSTREAM_MAIN;
			m_tMainSimCapSet.Clear(); 
			m_tSecondSimCapSet.Clear(); 
			m_tDStreamCap.Reset();
		}
	}
	/*
	#ifndef WIN32
	__attribute__ ( (packed) ) 
	#endif
	*/
	PACKED
	;

	//���廭��ϳɳ�Ա�ṹ (len:8)
	struct TVMPMember: public TMt
	{
	protected:
		u8   m_byMemberType;     //����ϳɳ�Ա����,�μ�mcuconst.h�л���ϳɳ�Ա���Ͷ���
		u8   m_byMemStatus;      //��Ա״̬(MT_STATUS_CHAIRMAN, MT_STATUS_SPEAKER, MT_STATUS_AUDIENCE)

	public:

		void  SetMemberType(u8   byMemberType){ m_byMemberType = byMemberType;} 
		u8    GetMemberType( void ) const { return m_byMemberType; }
		void  SetMemStatus(u8 byStatus) { m_byMemStatus = byStatus; }
		u8    GetMemStatus(void) const { return m_byMemStatus; }
		void  SetMemberTMt( TMt tMt ){ memcpy( this, &tMt, sizeof(TMt) ); }

		void Clear(void)
		{
			SetNull();
			SetMemberType( 0 );
			SetMemStatus( 0 );
		}
	}
	/*
	#ifndef WIN32
	__attribute__ ( (packed) ) 
	#endif
	*/
	PACKED
	;

	//�����ϵ�tl2.6
	struct TVMPParam
	{
	protected:
		u8      m_byIsCping;      //�Ƿ����ڻ���ϳ� 
		u8      m_byVMPBrdst;     //�ϳ�ͼ���Ƿ����ն˹㲥 0-�� 1-�� 
		u8      m_byVMPStyle;     //����ϳɷ��,�μ�mcuconst.h�л���ϳɷ����
		u8      m_byVMPSchemeId;  //�ϳɷ�񷽰����,���֧��5�׷���,1-5
		u8      m_byVMPMode;      //ͼ�񸴺Ϸ�ʽ: 0-��ͼ�񸴺� 1-��ػ���ϯ����ͼ�񸴺� 2-�Զ�ͼ�񸴺�(��̬���������ó�Ա)
		u8		m_byRimEnabled;	  //�Ƿ�ʹ�ñ߿�: 0-��ʹ��(Ĭ��) 1-ʹ��;
		u8		m_byAddMmbAlias;  //�ϳ�ͼ���Ƿ���Ӻϳɳ�Ա����, zgc, 2009-07-21
		u8		m_byReserved2;	  //�����ֶ�
		v26::TVMPMember  m_atVMPMember[max_vmp_mem]; //����ϳɳ�Ա
	public:

		void Clear(void)
		{
			memset( this, 0, sizeof(TVMPParam) );
			SetVMPMode( CONF_VMPMODE_NONE );
			SetIsCping( FALSE );
			for ( u8 byIdx = 0; byIdx < max_vmp_mem; byIdx++ )
			{
				m_atVMPMember[byIdx].Clear();
			}
			return;
		}

		void   SetIsCping( BOOL32 bIsCping ) { m_byIsCping = bIsCping ? 1 : 0; }
		BOOL32 IsCping( void ) const { return ( 0 == m_byIsCping ) ? FALSE : TRUE; }
		void   SetVMPBrdst(u8   byVMPBrdst){ m_byVMPBrdst = byVMPBrdst;} 
		BOOL32   IsVMPBrdst( void ) const { return m_byVMPBrdst == 0 ? FALSE : TRUE; }
		void   SetVMPStyle(u8   byVMPStyle){ m_byVMPStyle = byVMPStyle;} 
		u8     GetVMPStyle( void ) const { return m_byVMPStyle; }
		void   SetVMPSchemeId(u8 bySchemeId) { m_byVMPSchemeId = bySchemeId; }
		u8     GetVMPSchemeId(void) const { return m_byVMPSchemeId; }
		void   SetVMPMode(u8   byVMPMode){ m_byVMPMode = byVMPMode;} 
		u8     GetVMPMode( void ) const { return m_byVMPMode; }
		void   SetIsRimEnabled(u8 byEnabled){ m_byRimEnabled = byEnabled; }
		BOOL32 GetIsRimEnabled(void) const { return m_byRimEnabled == 1 ? TRUE : FALSE; }
		void   SetIsAddMmbAlias( BOOL32 bAdd ) { m_byAddMmbAlias = bAdd ? 1 : 0; }
		BOOL32 IsAddMmbAlias(void) const { return ( 1 == m_byAddMmbAlias ) ? TRUE : FALSE; }

		void   SetVmpMember( u8   byMemberIdx, const v26::TVMPMember &tVMPMember )
		{
			if( byMemberIdx >= max_vmp_mem )return;
			m_atVMPMember[byMemberIdx] = tVMPMember;
		}
		void   ClearVmpMember( u8 byMemberIdx )
		{
			if( byMemberIdx >= max_vmp_mem )return;
			m_atVMPMember[byMemberIdx].SetNull();
			m_atVMPMember[byMemberIdx].SetMemberType(0);
			m_atVMPMember[byMemberIdx].SetMemStatus(0);
		}
		const v26::TVMPMember *GetVmpMember( u8   byMemberIdx ) const
		{
			if( byMemberIdx >= max_vmp_mem )return NULL;
			return &m_atVMPMember[byMemberIdx];
		}
	}
	PACKED
	;


	//5.0֮ǰ�ϵĽṹ��
	//�ýṹΪ�ն��ټ�������������Ϣ��
	struct TMiniConfInfo
	{
	protected:
		CConfId			m_cConfId;							//�����
		u16				m_wDuration;						//����ʱ��(����)
		u16				m_wBitRate;							//��������
		u8				m_byOpenMode;						//���鿪�ŷ�ʽ: 0-������,�ܾ��б�������ն� 1-����������� 2-��ȫ����
		u8				m_byEncryptMode;					//�������ģʽ: 0-������, 1-des����,2-aes����
		u8				m_byReleaseMode;					//���������ʽ: 0-�����Զ����� 1-���ն�ʱ�Զ�����
		u8				m_byIsOccupyVPU;					//�Ƿ�ռ��ͼ����
		u8				m_byOccupyVpuType;					//ռ��ͼ��������������
		u8				m_byCascadeMode;					//����ģʽ
		u8				m_byDiscussConf;					//��������ģʽ
		u8				m_byAllInitDumb;					//�Ƿ��ʼ����
		v26::TCapSupport m_tCapSupport;						//����֧�ֵ�ý��
		TMediaEncrypt	m_tMediaKey;						//��Կ
		v26::TVMPParam  m_tVMPParam;						//����ϳɲ���
		s8				m_achConfPwd[maxlen_pwd + 1];	    //��������
		s8				m_achConfName[maxlen_confname + 1];	//������
	public:
		void Clear(void)
		{
			memset( this, 0, sizeof(TMiniConfInfo) );
			return;
		}
		CConfId GetConfId( void ) const { return m_cConfId; }
		void    SetConfId( CConfId cConfId ){ m_cConfId = cConfId; }
		void   SetDuration(u16  wDuration){ m_wDuration = htons(wDuration);} 
		u16    GetDuration( void ) const { return ntohs(m_wDuration); }
		void   SetBitRate(u16  wBitRate){ m_wBitRate = htons(wBitRate);} 
		u16    GetBitRate( void ) const { return ntohs(m_wBitRate); }
		void   SetOpenMode(u8   byOpenMode){ m_byOpenMode = byOpenMode;} 
		u8     GetOpenMode( void ) const { return m_byOpenMode; }
		void   SetEncryptMode(u8   byEncryptMode){ m_byEncryptMode = byEncryptMode;} 
		u8     GetEncryptMode( void ) const { return m_byEncryptMode; }
		void   SetReleaseMode(BOOL32 bReleaseMode){ m_byReleaseMode = bReleaseMode;} 
		BOOL32   IsReleaseNoMt( void ) const { return m_byReleaseMode == 0 ? FALSE : TRUE; }
		void   SetCapSupport(const v26::TCapSupport& tCapSupport){ m_tCapSupport = tCapSupport;} 
		const v26::TCapSupport& GetCapSupport( void ) const { return m_tCapSupport; }
		const TMediaEncrypt& GetMediaKey(void) const { return m_tMediaKey; };
		void SetMediaKey(TMediaEncrypt& tMediaEncrypt){ memcpy(&m_tMediaKey, &tMediaEncrypt, sizeof(tMediaEncrypt));}
		void          SetVmpParam(const v26::TVMPParam& tVMPParam){ m_tVMPParam = tVMPParam;} 
		const v26::TVMPParam& GetVmpParam(void) const { return m_tVMPParam; }
		void   SetConfPwd( LPCSTR lpszConfPwd )
		{
			if( lpszConfPwd != NULL )
			{
				strncpy( m_achConfPwd, lpszConfPwd, sizeof( m_achConfPwd ) );
				m_achConfPwd[sizeof( m_achConfPwd ) - 1] = '\0';
			}
			else
			{
				memset( m_achConfPwd, 0, sizeof( m_achConfPwd ) );
			}
		}
		LPCSTR GetConfPwd( void ) const { return m_achConfPwd; }
		void   SetConfName( LPCSTR lpszConfName )
		{
			if( lpszConfName != NULL )
			{
				strncpy( m_achConfName, lpszConfName, sizeof( m_achConfName ) );
				m_achConfName[sizeof( m_achConfName ) - 1] = '\0';
			}
			else
			{
				memset( m_achConfName, 0, sizeof( m_achConfName ) );
			}
		}
		LPCSTR GetConfName( void ) const { return m_achConfName; }

		void SetIsOccupyVPU( BOOL32 bIsOccupyVPU )
		{
			m_byIsOccupyVPU = bIsOccupyVPU ? 1 : 0;
		}
		BOOL32 IsOccupyVPU( void ) const 
		{
			return ( m_byIsOccupyVPU == 0 ) ? FALSE : TRUE;
		}

		void SetOccupyVpuType( u8 byOccupyType )
		{
			m_byOccupyVpuType = byOccupyType;
		}

		u8 GetOccupyVpuType( void ) const
		{
			return m_byOccupyVpuType;
		}

		void SetSupportCascade( BOOL32 bSupportCascade)
		{
			m_byCascadeMode = (TRUE == bSupportCascade ? 1 : 0);
		}

		BOOL32 IsSupportCascade( void ) const
		{
			return (0 != m_byCascadeMode ? TRUE : FALSE);
		}

		void SetDiscussConf( BOOL32 bDiscuss )
		{
			m_byDiscussConf = (TRUE == bDiscuss ? 1 : 0);
		}
		BOOL32 IsDiscussConf( void ) const
		{
			return ( 0 != m_byDiscussConf ? TRUE : FALSE);
		}

		void SetAllInitDumb( BOOL32 bInitDumb )
		{
			m_byAllInitDumb = (TRUE == bInitDumb ? 1 : 0);
		}
		BOOL32 IsAllInitDumb( void ) const
		{
			return ( 0 != m_byAllInitDumb ? TRUE : FALSE);
		}
	}
	PACKED;

	//5.0֮ǰ�ϵĽṹ
	struct TCreateConfReq
	{
	private:
		u8  m_byMtType;
		s8  m_achMtE164[maxlen_e164 + 1];		  //�����ն˵�e164
		s8  m_achPhoneNumber[maxlen_alias + 1]; //����ǵ绰�ն���Ҫ�绰�ն˺���
		u8  m_bySatDCastMode;                 //���Ƿ�ɢ���飺0-��֧�֣�1-֧��
		v26::TMiniConfInfo m_tMiniConfInfo;
		TMiniConfInfoExt  m_tConfInfoExt;
		s8  m_achReserv[maxlen_reserver];     //Ԥ���ռ�

	public:
		TCreateConfReq()
		{
			Clear();
		}

		void Clear()
		{
			m_byMtType = 0;
			memset( &m_tMiniConfInfo, 0, sizeof( m_tMiniConfInfo ) );
			memset( m_achReserv, 0, sizeof( m_achReserv ) );
			memset( m_achMtE164, 0, sizeof( m_achMtE164 ) );
			memset( m_achPhoneNumber, 0, sizeof( m_achPhoneNumber ) );
			m_tConfInfoExt.Clear();
		}

		TMiniConfInfoExt GetConfInfoExt() const
		{
			return m_tConfInfoExt;
		}
		void SetConfInfoExt( const TMiniConfInfoExt &tConfInfoExt )
		{
			memcpy( &m_tConfInfoExt, &tConfInfoExt, sizeof( TMiniConfInfoExt ) );
		}

		u8 GetStaDCastMode() const 
		{ 
			return m_bySatDCastMode; 
		}

		void SetStaDCastMode( u8 bySatDCastMode )
		{
			m_bySatDCastMode = bySatDCastMode; 
		}

		void SetMtType( u8 byMtType )
		{
			m_byMtType = byMtType;
		}
		u8  GetMtType()	const
		{
			return m_byMtType;
		}

		void SetConfInfo( const v26::TMiniConfInfo &tConfInfo )
		{
			memcpy( &m_tMiniConfInfo, &tConfInfo, sizeof( m_tMiniConfInfo ) );
		}

		const v26::TMiniConfInfo& GetConfInfo() const
		{
			return m_tMiniConfInfo;
		}

		void SetMtE164( const s8 *pchMtE164 )
		{
			if ( NULL == pchMtE164 )
			{
				return;
			}
			memset( m_achMtE164, 0, sizeof(m_achMtE164) );
			strncpy( m_achMtE164, pchMtE164, sizeof( m_achMtE164 ) - 1 );
		}

		const s8 *GetMtE164() const 
		{
			return m_achMtE164;
		}

		void SetPhoneNumber( const s8 *pchPhoneNum )
		{
			if ( NULL == pchPhoneNum )
			{
				return;
			}
			memset( m_achPhoneNumber, 0, sizeof( m_achPhoneNumber ) );
			strncpy( m_achPhoneNumber, pchPhoneNum, sizeof( m_achPhoneNumber ) - 1 );
		}

		const s8 *GetPhoneNumber() const
		{
			return m_achPhoneNumber;
		}
	}
	PACKED
		;
	//end

	//�������Ϣ
	struct TConfNameInfo
	{
		CConfId		m_cConfId;          //�����
		char		m_achConfName[maxlen_confname + 1];		//������
		s8			m_achConfE164[maxlen_e164 + 1];		  //E164����
		u8			m_byMtNum;						  //����ն���
		u16			m_wRemainTime;					  //����ʣ��ʱ��

	public:
		TConfNameInfo(void)
		{
			SetNull();
		}
		void SetNull()
		{
			m_cConfId.SetNull();
			memset(m_achConfName, 0, sizeof(m_achConfName));
			SetConfE164(NULL, 0);
			SetMtNum(0);
			SetRemainTime(0);
		}
		void SetConfId(const CConfId &cConfId)
		{
			m_cConfId.SetConfId(cConfId);
		}
		CConfId GetConfId(void) const
		{
			return m_cConfId;
		}
		void   SetConfName(LPCSTR lpszConfName)
		{
			if (lpszConfName != NULL)
			{
				strncpy(m_achConfName, lpszConfName, sizeof(m_achConfName) - 1);
				//m_achConfName[MAXLEN_CONFNAME - 1] = '\0';
				m_achConfName[sizeof(m_achConfName) - 1] = '\0';
			}
			else
			{
				memset(m_achConfName, 0, sizeof(m_achConfName));
			}
		}
		LPCSTR GetConfName(void) const { return m_achConfName; }

		// ���û���E164����
		void SetConfE164(const s8 * psConfE164, u8 byLen)
		{
			memset(m_achConfE164, 0, sizeof(m_achConfE164));
			if (NULL == psConfE164 || 0 == byLen)
			{
				return;
			}
			u8 byRealLen = getmin(byLen, sizeof(m_achConfE164) - 1);
			memcpy(m_achConfE164, psConfE164, byRealLen);
			return;
		}
		// ��ȡ����E164����
		const s8 * GetConfE164(void) const
		{
			return m_achConfE164;
		}
		// ��������ն���
		void SetMtNum(u8 byMtNum)
		{
			m_byMtNum = byMtNum;
		}
		// ��ȡ����ն���
		u8   GetMtNum(void) const
		{
			return m_byMtNum;
		}
		// ���û���ʣ��ʱ��
		void SetRemainTime(u16 wRemainTime)
		{
			m_wRemainTime = htons(wRemainTime);
		}
		// ��ȡ����ʣ��ʱ��
		u16  GetRemainTime(void) const
		{
			return ntohs(m_wRemainTime);
		}
	}
	PACKED;

	struct TSimpleConfInfo
	{
		CConfId		m_cConfId;                         //�����
		char		m_achConfName[maxlen_confname + 1];  //������
		s8			m_achConfE164[maxlen_e164 + 1];      //E164����
		u8			m_byMtNum;						   //����ն���
		u16			m_wRemainTime;					   //����ʣ��ʱ��
		u8          m_byNeedPsw;                       //�Ƿ���Ҫ���룺0 - ����Ҫ��1 - ��Ҫ
		u8          m_byConfMode;					   //����ģʽ��0 - ���壬1 - ����, 2 - ���� ,3 - �Զ���
		TKdvTime    m_tStartTime;                      //���鿪ʼʱ��
		u8          m_bySatDCastMode;                  //add shenbiao[2013-7-17]�Ƿ�֧�����Ƿ�ɢ���飺0-��֧�֣�1-֧��
		u8          m_byPublicConf;                    //add rnn[2013/11/12]�Ƿ�Ϊ���Ż��飺0-���ǣ�1-��
		u8			m_byOpenMode;					   //rnn 5.0�������ԣ�0-���ػ��飻1-��ջ��飻2-��������
		u8          m_abyReserverd[54];                //�����ֽ�

	public:
		TSimpleConfInfo(void)
		{
			m_cConfId.SetNull();
			memset(m_achConfName, 0, sizeof(m_achConfName));
			SetConfE164(NULL, 0);
			SetMtNum(0);
			SetRemainTime(0);
			SetNeedPsw(0);
			SetSatDCastMode(FALSE);
			SetPublicConf(1);
			SetOpenMode(CONF_OPENMODE_OPEN);
			memset(m_abyReserverd, 0, sizeof(m_abyReserverd));
		};

		//startTime
		TKdvTime GetStartTime(void) const
		{
			return m_tStartTime;
		}
		void SetStartTime(TKdvTime tStartTime)
		{
			m_tStartTime = tStartTime;
		}

		void SetConfId(const CConfId &cConfId)
		{
			m_cConfId.SetConfId(cConfId);
		}
		CConfId GetConfId(void)
		{
			return m_cConfId;
		}
		void   SetConfName(LPCSTR lpszConfName)
		{
			if (lpszConfName != NULL)
			{
				strncpy(m_achConfName, lpszConfName, sizeof(m_achConfName) - 1);
				m_achConfName[sizeof(m_achConfName) - 1] = '\0';
			}
			else
			{
				memset(m_achConfName, 0, sizeof(m_achConfName));
			}
		}
		LPCSTR GetConfName(void) const
		{
			return m_achConfName;
		}

		// ���û���E164����
		void SetConfE164(const s8 * psConfE164, u8 byLen)
		{
			memset(m_achConfE164, 0, sizeof(m_achConfE164));
			if (NULL == psConfE164 || 0 == byLen)
			{
				return;
			}
			u8 byRealLen = getmin(byLen, sizeof(m_achConfE164) - 1);
			memcpy(m_achConfE164, psConfE164, byRealLen);
			return;
		}
		// ��ȡ����E164����
		const s8 * GetConfE164(void) const
		{
			return m_achConfE164;
		}

		//�ն���
		void SetMtNum(u8 byMtNum)
		{
			m_byMtNum = byMtNum;
		}
		u8   GetMtNum(void)
		{
			return m_byMtNum;
		}

		//����ģʽ
		void SetConfMode(u8 byConfMode)
		{
			m_byConfMode = byConfMode;
		}
		u8   GetConfMode(void)
		{
			return m_byConfMode;
		}


		// ���û���ʣ��ʱ��
		void SetRemainTime(u16 wRemainTime)
		{
			m_wRemainTime = htons(wRemainTime);
		}
		// ��ȡ����ʣ��ʱ��
		u16  GetRemainTime(void) const
		{
			return ntohs(m_wRemainTime);
		}

		//m_byNeedPsw
		void SetNeedPsw(BOOL32 bNeedPsw)
		{
			m_byNeedPsw = (bNeedPsw ? 1 : 0);
		}
		BOOL32 IsNeedPsw(void)
		{
			return ((0 == m_byNeedPsw) ? FALSE : TRUE);
		}

		void SetSatDCastMode(BOOL32 bSatDCastMode)
		{
			if (bSatDCastMode)
			{
				m_bySatDCastMode = 1;
			}
			else
			{
				m_bySatDCastMode = 0;
			}
		}
		BOOL32 IsSatDCastMode() const
		{
			return m_bySatDCastMode == 0 ? FALSE : TRUE;
		}

		void SetPublicConf(u8 byPublicConf)
		{
			m_byPublicConf = byPublicConf;
		}

		u8   GetPublicConf(void)
		{
			return m_byPublicConf;
		}

		void SetOpenMode(u8 byOpenMode)
		{
			m_byOpenMode = byOpenMode;
		}

		u8   GetOpenMode(void)
		{
			return m_byOpenMode;
		}

		void Print(void)
		{
			OspPrintf(TRUE, FALSE, "\n\n");
			OspPrintf(TRUE, FALSE, "Conf Name is %s\n", GetConfName());
			OspPrintf(TRUE, FALSE, "Conf E164 is %s\n", GetConfE164());
			OspPrintf(TRUE, FALSE, "Conf MtNum is %d\n", GetMtNum());
			OspPrintf(TRUE, FALSE, "Conf RemainTime is %d\n", GetRemainTime());
			OspPrintf(TRUE, FALSE, "Conf NeedPsw is %d\n", IsNeedPsw());
			OspPrintf(TRUE, FALSE, "Conf ConfMode is %d\n", GetConfMode());
			OspPrintf(TRUE, FALSE, "Conf PublicAttrb is %d\n", GetPublicConf());
			OspPrintf(TRUE, FALSE, "Conf Open Mode is %d\n", GetOpenMode());

			s8 achTimeBuffer[40] = { 0 };
			m_tStartTime.GetString(achTimeBuffer, sizeof(achTimeBuffer));
			OspPrintf(TRUE, FALSE, "Conf StartTime is %s\n\n", achTimeBuffer);

			if (IsSatDCastMode())
			{
				OspPrintf(TRUE, FALSE, "Conf is Stat conf\n\n");
			}
			else
			{
				OspPrintf(TRUE, FALSE, "Conf is not Stat conf\n\n");
			}
		}
	}
	PACKED;

	// wy[2014/12/04] Ϊ���NU͸�����ն�Ͷ�ݵ���Ϣ���Ӵ˽ṹ�壬ֻ���ն˻�ȡ��ϸ��Ϣʱʹ��
	struct TNuConfE164
	{
		u32 m_byType;
		s8 m_achAlias[64 + 2];

		TNuConfE164()
		{
			Clear();
		}

		void Clear()
		{
			m_byType = 0;
			memset(m_achAlias, 0, sizeof(m_achAlias));
		}

		//���ñ���
		// ֻget���ʲ��ṩset�ӿ�
		// 	BOOL SetAliasAddr( u8 byType, const char* pAlias )
		// 	{
		// 		if ( pAlias == NULL ) 
		// 			return FALSE;
		// 		
		// 		memset( m_achAlias, 0, sizeof(m_achAlias) );
		// 		
		// 		m_byType = aliasTbyTypeype;
		// 		
		// 		u16 nLen = min( strlen(pAlias), 64 );
		// 		strncpy( m_achAlias, pAlias, nLen );
		// 		m_achAlias[nLen] = '\0';
		// 		return TRUE;
		// 	}	

		//��ñ���
		s8* GetAliasName()
		{
			return m_achAlias;
		}
	}PACKED;
}

#ifdef WIN32
#pragma pack( pop )
#endif

static void UpdateV26SimCapSet(TSimCapSet& tSimCapSet)
{
	if (tSimCapSet.GetVideoResolution() == 14)
	{
		tSimCapSet.SetVideoResolution(VIDEO_FORMAT_WCIF);
	}
	else if (tSimCapSet.GetVideoResolution() == 36)
	{
		tSimCapSet.SetVideoResolution(VIDEO_FORMAT_WXGA);
	}
}
static void ConvertStruct(const v26::TVMPMember& tOld, TVMPMember& tNew)
{
	tNew.SetChnIdx(0);
	tNew.SetMemberType(tOld.GetMemberType());
	tNew.SetMemStatus(tOld.GetMemStatus());
	tNew.SetMemberTMt(tOld);
}
static void ConvertStruct(const v26::TCapSupport& tOld, TCapSupport& tNew)
{
	//2.6�ն˷ֱ��ʶ����ƽ̨��һ�£���Ҫ������
	//��WCIFֵΪ14����ҪתΪ15
	//WXGAΪ36��ҪתΪ14�������ն˵�ʱ���򷴹���

	tNew.SetSupportMMcu(tOld.IsSupportMMcu());
	tNew.SetSupportH224(tOld.IsSupportH224());
	tNew.SetSupportT120(tOld.IsSupportT120());
	tNew.SetEncryptMode(tOld.GetEncryptMode());
	tNew.SetDStreamType(tOld.GetDStreamType());
	TDStreamCap tDStreamCapSet;
	tDStreamCapSet = tOld.GetDStreamCapSet();
	if (tDStreamCapSet.GetResolution() == 14)
	{
		tDStreamCapSet.SetResolution(VIDEO_FORMAT_WCIF);
	}
	else if (tDStreamCapSet.GetResolution() == 36)
	{
		tDStreamCapSet.SetResolution(VIDEO_FORMAT_WXGA);
	}
	tNew.SetDStreamCapSet(tDStreamCapSet);

	TSimCapSet tMainSimCapSet;
	tMainSimCapSet = tOld.GetMainSimCapSet();
	UpdateV26SimCapSet(tMainSimCapSet);
	if (0 == tMainSimCapSet.GetVideoFrameRate())
	{
		tMainSimCapSet.SetVideoFrameRate( (u8)30 );
	}
	tNew.SetMainSimCapSet(tMainSimCapSet);

	TSimCapSet tSecondSimCapSet;
	tSecondSimCapSet = tOld.GetSecondSimCapSet();
	UpdateV26SimCapSet(tSecondSimCapSet);
	tNew.SetSecondSimCapSet(tSecondSimCapSet);
}
static void ConvertStruct(const v26::TVMPParam& tOld, TVMPParam& tNew)
{
	tNew.SetIsCping(tOld.IsCping());
	tNew.SetVMPAuto((tOld.GetVMPMode() == CONF_VMPMODE_AUTO) ? TRUE : FALSE);
	tNew.SetVMPBrdst(tOld.IsVMPBrdst());
	tNew.SetVMPStyle(tOld.GetVMPStyle());
	tNew.SetVMPSchemeId(tOld.GetVMPSchemeId());
	tNew.SetVMPMode(tOld.GetVMPMode());
	tNew.SetIsRimEnabled(tOld.GetIsRimEnabled());
	tNew.SetIsAddMmbAlias(tOld.IsAddMmbAlias());
	u8 byVmpMemIdx = 0;
	for (; byVmpMemIdx < v26::max_vmp_mem; byVmpMemIdx++)
	{
		TVMPMember tVmpMember;
		ConvertStruct(*tOld.GetVmpMember(byVmpMemIdx), tVmpMember);
		tNew.SetVmpMember(byVmpMemIdx, tVmpMember);
	}
}
static void ConvertStruct(const v26::TMiniConfInfo& tOld, TMiniConfInfo& tNew)
{
	tNew.SetConfId(tOld.GetConfId());
	tNew.SetDuration(tOld.GetDuration());
	tNew.SetBitRate(tOld.GetBitRate());
	tNew.SetOpenMode(tOld.GetOpenMode());
	tNew.SetEncryptMode(tOld.GetEncryptMode());
	tNew.SetReleaseMode(tOld.IsReleaseNoMt());
	tNew.SetIsOccupyVPU(tOld.IsOccupyVPU());
	tNew.SetOccupyVpuType(tOld.GetOccupyVpuType());
	tNew.SetSupportCascade(tOld.IsSupportCascade());
	tNew.SetDiscussConf(tOld.IsDiscussConf());
	tNew.SetAllInitDumb(tOld.IsAllInitDumb());
	tNew.SetMediaKey(tOld.GetMediaKey());
	tNew.SetConfPwd(tOld.GetConfPwd());

	TVMPParam tVMPParam;
	v26::TVMPParam tVMPParam_Old;
	tVMPParam_Old = tOld.GetVmpParam();
	ConvertStruct(tVMPParam_Old, tVMPParam);
	//	tNew.SetVmpParam(tVMPParam);


	TCapSupport	tCapSupport;
	v26::TCapSupport tCapSupport_Old;
	tCapSupport_Old = tOld.GetCapSupport();
	ConvertStruct(tCapSupport_Old, tCapSupport);
	tNew.SetCapSupport(tCapSupport);

	// ����ҵ�ն�ֻ֧��GBK��������Ҫ�Ի������ƽ���ת��
	s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
	u32 dwDstLen = gb2312_to_utf8(tOld.GetConfName(), achConfName, sizeof(achConfName) - 1);
	tNew.SetConfName(achConfName);
}

static void ConvertStruct(const TConfBaseInfo& tNew, v26::TConfBaseInfo& tOld)
{
	// ����ҵ�ն�ֻ֧��GBK��������Ҫ����ת��
	utf8_to_gb2312(tNew.m_achConfName, tOld.m_achConfName, sizeof(tOld.m_achConfName) - 1);
	strncpy(tOld.m_achConfE164, tNew.m_achConfE164, sizeof(tOld.m_achConfE164) - 1);
	tOld.m_achConfE164[sizeof(tOld.m_achConfE164) - 1] = 0;
	memcpy(tOld.m_achDomainGUID, tNew.m_achDomainGUID, sizeof(tOld.m_achDomainGUID));
	utf8_to_gb2312(tNew.m_achDomainName, tOld.m_achDomainName, sizeof(tOld.m_achDomainName) - 1);
	memcpy(tOld.m_achDomainMOID, tNew.m_achDomainMOID, sizeof(tOld.m_achDomainMOID));
}

static void ConvertStruct(const TConfDetailInfo& tNew, v26::TConfDetailInfo& tOld)
{
	ConvertStruct(tNew.m_tNewConfBaseInfo, tOld.m_tNewConfBaseInfo);

	tOld.m_byConfStyle = tNew.m_byConfStyle;
	strncpy(tOld.m_achShortNo, tNew.m_achShortNo, sizeof(tOld.m_achShortNo) - 1);
	tOld.m_achShortNo[sizeof(tOld.m_achShortNo) - 1] = 0;
	tOld.m_byEncryptMode = tNew.m_byEncryptMode;
	tOld.m_byConfMode = tNew.m_byConfMode;
	tOld.m_wBitRate = tNew.m_wBitRate;
	if (VIDEO_FORMAT_WXGA == tNew.m_byResolution)
	{
		tOld.m_byResolution = 36;
	}
	else if (VIDEO_FORMAT_WCIF == tNew.m_byResolution)
	{
		tOld.m_byResolution = 14;
	}
	else
	{
		tOld.m_byResolution = tNew.m_byResolution;
	}

	if (tNew.IsNeedPsw())
	{
		tOld.m_byOpenMode = CONF_OPENMODE_NEEDPWD;
	}
	else
	{
		tOld.m_byOpenMode = tNew.m_byOpenMode;
	}

	tOld.m_byDiscussConf = tNew.m_byMixMode;
	tOld.m_tStartTime = tNew.m_tStartTime;
	// ����ҵ�ն�δ�����ֽ���ת��
	tOld.m_wDuration = tNew.GetDuration();
	// ����ҵ�ն�ֻ֧��GBK��������Ҫ�Դ��������ƽ���ת��
	utf8_to_gb2312(tNew.m_achAlias, tOld.m_achAlias, sizeof(tOld.m_achAlias) - 1);
	tOld.m_byDualMode = tNew.m_byDualMode;
	tOld.m_bySatDCastMode = tNew.m_bySatDCastMode;
	tOld.m_byPublicConf = tNew.m_byPublicConf;
}

//v26::TCreateConfReqת��ΪTCreateConfReq
static void ConvertStruct(const v26::TCreateConfReq& tOld, TCreateConfReq& tNew)
{
	tNew.Clear();
	tNew.SetMtType(tOld.GetMtType());
	tNew.SetMtE164(tOld.GetMtE164());
	tNew.SetPhoneNumber(tOld.GetPhoneNumber());
	tNew.SetStaDCastMode(tOld.GetStaDCastMode());

	v26::TMiniConfInfo tMiniConfInfo_Old;
	TMiniConfInfo tMiniConfInfo;
	tMiniConfInfo_Old = tOld.GetConfInfo();
	ConvertStruct(tMiniConfInfo_Old, tMiniConfInfo);
	TMiniConfInfoExt tMiniConfInfoExt;
	tMiniConfInfoExt = tOld.GetConfInfoExt();
	tMiniConfInfo.SetPublicConf(tMiniConfInfoExt.GetPublicConf());
	tMiniConfInfo.SetAudioScope(tMiniConfInfoExt.GetAudioScope());
	tNew.SetConfInfo(tMiniConfInfo);
	return;
}



static void ConvertStruct(const TSimpleConfInfo& tIn, v26::TSimpleConfInfo& tOut)
{
	tOut.SetConfE164(tIn.GetConfE164(), strlen(tIn.GetConfE164()));
	tOut.SetConfId(tIn.GetConfId());
	tOut.SetConfMode(tIn.GetConfMode());
	tOut.SetMtNum(tIn.GetMtNum());
	tOut.SetNeedPsw(tIn.IsNeedPsw());
	tOut.SetPublicConf(tIn.GetPublicConf());
	tOut.SetStartTime(tIn.GetStartTime());
	tOut.SetRemainTime(tIn.GetRemainTime());
	tOut.SetOpenMode(tIn.GetOpenMode());
	tOut.SetSatDCastMode(tIn.IsSatDCastMode());

	s8 achConfName[v26::maxlen_confname + 1] = { 0 };
	u32 dwDstLen = utf8_to_gb2312(tIn.GetConfName(), achConfName, sizeof(achConfName) - 1);
	if (0 == dwDstLen)
	{
		//OspPrint(TRUE, FALSE, "[ConvertStruct::TSimpleConfInfo]utf8_to_gb2312 trans DstLen == 0, error!\n");
	}
	else
	{
		tOut.SetConfName(achConfName);
	}
}
static void ConvertStruct(const TSimpleConfInfo& tIn, v26::TConfNameInfo& tOut)
{
	s8 achConfName[v26::maxlen_confname + 1] = { 0 };
	u32 dwDstLen = utf8_to_gb2312(tIn.GetConfName(), achConfName, sizeof(achConfName) - 1);
	tOut.SetConfName(achConfName);
	tOut.SetConfE164(tIn.GetConfE164(), strlen(tIn.GetConfE164()));
	tOut.SetRemainTime(tIn.GetRemainTime());
	tOut.SetConfId(tIn.GetConfId());
	tOut.SetMtNum(tIn.GetMtNum());
}