#ifndef _GKMMCUERR_H_
#define _GKMMCUERR_H_
#define GKM_MCU_REQ_WRONGPARAMS			1	/*ע�������������*/
#define GKM_MCU_RESOURCE_UNAVAILABLE	2	/* GRJ, RRJ, ARJ, LRJ������Դ�ľ� */
#define GKM_MCU_INSUFFICIENT_BAND		3	/* BRJ������ */
#define GKM_MCU_INVALID_REVISION		4	/* GRJ, RRJ, BRJ ���Ϸ����޸�*/
#define GKM_MCU_INVALID_CALLADDR		5	/* RRJ ���Ϸ��ĺ��е�ַ*/
#define GKM_MCU_INVALID_RASADDR			6	/* RRJ ���Ϸ���RAS��ַ*/
#define GKM_MCU_INVALID_ENDPOINTTYPE	7       /* RRJ ���Ϸ����ն�����*/
#define GKM_MCU_INVALID_PERMISSION		8	/* ARJ - Ȩ�޲��Ϸ�*/
#define GKM_MCU_INVALID_CONFERENCEID	9	/* BRJ - ����ID*/
#define GKM_MCU_INVALID_ENDPOINTID		10	/*�ն�ID*/
#define GKM_MCU_CALLER_NOT_REGED		11	/* ARJδע�� */
#define GKM_MCU_CALLED_PARTYNOTREG		12	/* ARJ - ��ַ�������*/
#define GKM_MCU_DISCOVRY_REQUIRED		13	/* RRJ - ע����Ϣ��Ҫ����*/
#define GKM_MCU_DUPLICATE_ALIAS			14	/* RRJ - �����Ѿ�ע�� */
#define GKM_MCU_TRANSPORTNOTSUPPORTED	15	/* RRJ - �������*/
#define GKM_MCU_CALLINPROGRESS			16	/* URJ - ���ڴ�����*/
#define GKM_MCU_REQUESTTODROPOTHER		17	/* DRJ - ���ܹ��ͷ������Ĵ���*/
#define GKM_MCU_NOTREGISTERED			18	/* DRJ, LRJ, INAK - δע��*/
#define GKM_MCU_UNDEFINEDREASON			19	/* GRJ, RRJ, URJ, ARJ, BRJ, LRJ, INAK */
#define GKM_MCU_REASONNOTBOUND			20	/* BRJ - */
#define GKM_MCU_NOTCURRENTREGISTERED	21	/* URJ - ��ǰδע��*/
#define GKM_MCU_REQUESTDENIED			22	/* ARJ - δ���ô���*/
#define GKM_MCU_LOCATIONNOTFOUND		23	/* LRJ - δ���� */
#define GKM_MCU_SECURITYDENID			24	/* GRJ, RRJ, URJ, ARJ, BRJ, LRJ, DRJ, INAK */
#define GKM_MCU_QOSNOTSUPPORTED			25	/* RRJ */
#define GKM_MCU_INVALID_ALIAS			26	/* RRJ - �������Ϸ�*/
#define GKM_MCU_PERMISSIONDENIED		27	/* URJ - ���ܲ���*/
#define GKM_MCU_QOSCONTROLNOTSUPPORTED	28	/* ARJ */
#define GKM_MCU_INCOMPLETEADDR			29	/* ARJ, LRJ ��ַ��Ϣ������*/
#define GKM_MCU_FULLREGREQUIRED			30	/* RRJ */
#define GKM_MCU_ROUTECALLTOSCN			31	/* ARJ, LRJ */
#define GKM_MCU_ALIASINCONSISTENT		32	/* ARJ, LRJ - �������Ӧ����ģ�� */
#define GKM_MCU_REGNOTSUPPORTED			33	/* RRJ */
#define GKM_MCU_INVALID_TERMINALALIAS	34	/* RRJ */
#define GKM_MCU_EXCEEDCALLCAPACITY		35	/* ARJ - ��������*/
#define GKM_MCU_COLLECTDESTINATION		36	/* ARJ */
#define GKM_MCU_COLLECTPIN				37	/* ARJ */
#define GKM_MCU_GENERICDATA				38	/* GRJ, RRJ, ARJ, LRJ */
#define GKM_MCU_FEATURENOTSUPPORTED		39	/* GRJ, RRJ, ARJ, LRJ */
#define GKM_MCU_HOPCOUNTEXCEEDED		40	/* LRJ */

#define GKM_MCU_EXCEPTION				41  /*�������г����쳣*/
#define GKM_MCU_ERRSTATE				42  /*����״̬*/
#endif