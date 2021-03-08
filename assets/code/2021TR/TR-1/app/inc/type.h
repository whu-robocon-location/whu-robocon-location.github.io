#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C"
{
	#endif

	/*
	;*****************************************************************************************************
	;* 描    述 : 定义系统的数据类型。
	;*****************************************************************************************************
	;*/
	typedef unsigned char 		boolean;		/*无符字符型 				*/
	typedef unsigned char 		uint8; 			/*无符字符型 		Unsigned  8 bit quantity                            */
	typedef signed 	 char 		int8; 			/*字符型 				Signed    8 bit quantity                            */
	typedef unsigned short 		uint16; 		/*无符短整型 		Unsigned 16 bit quantity                            */
	typedef signed 	 short 		int16; 			/*短整型 				Signed   16 bit quantity                            */
	typedef unsigned int 			uint32; 		/*无符32位整型 	Unsigned 32 bit quantity                            */
	typedef signed 	 int 			int32; 			/*32位整型 			Signed   32 bit quantity                            */
	typedef float 						fp32; 			/*单精度型 			Single precision floating point                     */

	typedef unsigned char 		BYTE;				/*无符字符型 				*/
	typedef unsigned short 		WORD;				/*无符短整型 				*/
	typedef unsigned long 		DWORD;			/*无符长整型 				*/
	typedef unsigned int 			BOOL;				/*无符字符型 				*/

	typedef unsigned char 		uint8_t; 		/*无符字符型 		Unsigned  8 bit quantity                            */
	typedef signed 	 char 		int8_t; 		/*字符型 				Signed    8 bit quantity                            */
	typedef unsigned short 		uint16_t; 	/*无符短整型 		Unsigned 16 bit quantity                            */
	typedef signed 	 short 		int16_t; 		/*短整型 				Signed   16 bit quantity                            */
	typedef unsigned int 			uint32_t; 	/*无符32位整型 	Unsigned 32 bit quantity                            */
	typedef signed 	 int 			int32; 			/*32位整型 			Signed   32 bit quantity                            */


	typedef unsigned char 		u_int8_t; 	/*无符字符型 		Unsigned  8 bit quantity                            */
	typedef unsigned short 		u_int16_t; 	/*无符短整型 		Unsigned 16 bit quantity                            */
	typedef unsigned int 			u_int32_t; 	/*无符32位整型 	Unsigned 32 bit quantity                            */
	typedef signed 	 int 			int32_t; 		/*32位整型 			Signed 	 32 bit quantity                            */
	typedef __int64 					int64_t;		/*64位整型 					*/
	typedef unsigned __int64 	uint64_t;		/*64位无符整型 			*/
	typedef unsigned short 		u_short;
	

	typedef unsigned char 		u_char;			/*无符字符型 				*/

	typedef unsigned char 		u8_t;				/*无符字符型 				*/
	typedef signed 	 char 		s8_t;				/*字符型 						*/
	typedef unsigned short 		u16_t;			/*无符短整型 				*/
	typedef signed 	 short 		s16_t;			/*无符短整型 				*/
	typedef unsigned int 			u32_t;			/*无符32位整型 			*/
	typedef signed 	 int 			s32_t;			/*32位整型 					*/

//	typedef uint32_t 					u32;				/*无符32位整型 			*/
//	typedef uint16_t 					u16;				/*无符短整型 				*/
//	typedef uint8_t 					u8;					/*无符字符型 				*/

	typedef unsigned short 		__le16;			/*无符短整型 				*/
	typedef unsigned int 			__le32;			/*无符32位整型 			*/
	typedef unsigned __int64 	__le64;			/*64位无符整型 			*/

	typedef unsigned short		__be16;			/*无符短整型 				*/
	typedef unsigned int 			__be32;			/*无符32位整型 			*/
	//typedef unsigned __int64  __be64;
	typedef unsigned short 		__u16;			/*无符短整型 				*/
	typedef unsigned char 		__u8;				/*无符字符型 				*/
	typedef unsigned __int64 	u64; 				/*64位无符整型 			*/
	typedef unsigned int 			__u32;			/*无符32位整型 			*/

	typedef signed char 			__s8;				/*字符型 						*/
	typedef signed char 			s8;					/*字符型 						*/
	typedef signed short 			s16;				/*短整型 						*/
	typedef signed int 				s32;				/*32位整型 					*/
	//typedef  signed __int64  s64;

	typedef unsigned int 			size_t;			/*无符32位整型 			*/

	typedef unsigned char			UINT8;			/*无符字符型 				*/
	typedef unsigned short		UINT16;			/*无符短整型 				*/
	typedef unsigned int			UINT32;			/*无符32位整型 			*/
	typedef unsigned __int64	UINT64;			/*64位无符整型 			*/

	typedef char							INT8;				/*字符型 						*/
	typedef short							INT16;			/*短整型 						*/
	typedef int								INT32;			/*32位整型 					*/
	typedef __int64						INT64;			/*64位无符整型 			*/

		
	typedef unsigned char 		UCHAR;			/*无符字符型 				*/
	typedef unsigned short 		USHORT;			/*无符短整型 				*/
	typedef unsigned int 			UINT;				/*无符32位整型 			*/
	typedef unsigned long 		ULONG;			/*无符长整型 				*/

	typedef unsigned char 		*PUINT8;		/*无符字符型指针 		*/
	typedef unsigned short		*PUINT16;		/*无符短整型指针 		*/
	typedef unsigned int 	 		*PUINT32;		/*无符32位整型指针 	*/
	typedef unsigned __int64 	*PUINT64;		/*64位无符整型指针 	*/
	typedef int 							*PINT32;		/*32位整型指针 			*/
	typedef long long 				*PINT64;		/*64位无符整型指针 	*/

	/* modified for fixing compile warning on Sigma 8634 platform */
	typedef char STRING;

	typedef signed char 			CHAR;				/*字符型 						*/

	typedef signed short 			SHORT;			/*短整型 						*/
	typedef signed int 				INT;				/*32位整型 					*/
	typedef signed long 			LONG;				/*长整型 						*/
	typedef __int64 					LONGLONG;		/*64位无符整型 			*/

	typedef unsigned __int64 	ULONGLONG;	/*64位无符整型 			*/

	typedef unsigned char 		BOOLEAN;		/*无符字符型 				*/

	typedef void 		VOID;

	typedef char 						 *PSTRING;		/*字符型指针 				*/
	typedef VOID 						 *PVOID;			/*空指针 						*/
	typedef CHAR 						 *PCHAR;			/*字符型指针 				*/
	typedef UCHAR 					 *PUCHAR;			/*无符字符型指针 		*/
	typedef USHORT 					 *PUSHORT;		/*无符短整型指针 		*/
	typedef LONG 						 *PLONG;			/*长整型指针 				*/
	typedef ULONG 					 *PULONG;			/*无符长整型 				*/
	typedef UINT 						 *PUINT;			/*无符32位整型指针 	*/

	#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
	
	#define BITS_PER_LONG 32

	#define BIT(nr)						(1UL << (nr))
	#define BIT_MASK(nr)			(1UL << ((nr) % BITS_PER_LONG))
	#define BIT_WORD(nr)			((nr) / BITS_PER_LONG)
	#define BITS_PER_BYTE			8
	#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

	typedef enum
	{
		FALSE = 0, TRUE = !FALSE
	} bool;

	#define NETDEV_ALIGN				32
	#define ETHTOOL_BUSINFO_LEN	32

	typedef unsigned gfp_t;
	typedef unsigned fmode_t;

	#define __packed	__attribute__((packed))	//使变量或者结构体成员使用最小的对齐方式, 即对变量一字节对齐, 对域是位对齐

	#define false FALSE
	#define true  TRUE


	#ifndef NULL
	#define NULL    ((void *)0)
	#endif

	#ifdef __cplusplus
}

#endif

#endif
