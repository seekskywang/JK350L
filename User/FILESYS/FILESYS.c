#include "FILESYS.h"

/*******************************************************************************
* 函  数  名      : CH376ReadBlock
* 描      述      : 从当前主机端点的接收缓冲区读取数据块,.
* 输      入      : u8* buf:
*                   指向外部接收缓冲区.
* 返      回      : 返回长度.
*******************************************************************************/
u8  CH376ReadBlock( u8* buf )
{
    u8  s, l;

    xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
    s = l = xReadCH376Data( );                                                          /* 后续数据长度 */
    if ( l )
    {
        do
        {
            *buf = xReadCH376Data( );
            buf ++;
        }
        while ( -- l );
    }
    xEndCH376Cmd( );
    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376WriteReqBlock
* 描      述      : 向内部指定缓冲区写入请求的数据块,返回长度.
* 输      入      : u8* buf:
*                   指向发送缓冲区.
* 返      回      : u8 s：后续数据长度.
*******************************************************************************/
u8  CH376WriteReqBlock( u8* buf )
{
    u8  s, l;

    xWriteCH376Cmd( CMD01_WR_REQ_DATA );                                                /* 向内部指定缓冲区写入请求的数据块 */
    s = l = xReadCH376Data( );                                                          /* 后续数据长度 */
    if ( l )
    {
        do
        {
            xWriteCH376Data( *buf );
            buf ++;
        }
        while ( -- l );
    }
    xEndCH376Cmd( );
    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376WriteHostBlock
* 描      述      : 向USB主机端点的发送缓冲区写入数据块.
* 输      入      : u8* buf：
*                   指向发送数据缓冲区.
*                   u8 len:
*                   数据长度.
* 返      回      : 无.
*******************************************************************************/
void    CH376WriteHostBlock( u8* buf, u8 len )
{
    xWriteCH376Cmd( CMD10_WR_HOST_DATA );
    xWriteCH376Data( len );                                                             /* 后续数据长度 */
    if ( len )
    {
        do
        {
            xWriteCH376Data( *buf );
            buf ++;
        }
        while ( -- len );
    }
    xEndCH376Cmd( );
}

/*******************************************************************************
* 函  数  名      : CH376WriteOfsBlock
* 描      述      : 向内部缓冲区指定偏移地址写入数据块.
* 输      入      : u8* buf：
*                   指向发送数据缓冲区.
*                   u8 ofs:
*                   偏移地址.
*                   u8 len:
*                   数据长度.
* 返      回      : 无.
*******************************************************************************/
void    CH376WriteOfsBlock( u8* buf, u8 ofs, u8 len )
{
    xWriteCH376Cmd( CMD20_WR_OFS_DATA );
    xWriteCH376Data( ofs );                                                             /* 偏移地址 */
    xWriteCH376Data( len );                                                             /* 数据长度 */
    if ( len )
    {
        do
        {
            xWriteCH376Data( *buf );
            buf ++;
        }
        while ( -- len );
    }
    xEndCH376Cmd( );
}

/*******************************************************************************
* 函  数  名      : CH376SetFileName
* 描      述      : 设置将要操作的文件的文件名 .
* 输      入      : u8* name：
*                   指向文件名缓冲区.
* 返      回      : 无.
*******************************************************************************/
void    CH376SetFileName( u8* name )
{
    u8  c;

#ifndef DEF_IC_V43_U                                                                    /* 默认支持低版本 */
    u8  s;

    xWriteCH376Cmd( CMD01_GET_IC_VER );                                                 /* 获取芯片版本 */
    if (  xReadCH376Data( ) < 0x43 )
    {
        if ( CH376ReadVar8( VAR_DISK_STATUS ) < DEF_DISK_READY )
        {
            xWriteCH376Cmd( CMD10_SET_FILE_NAME );
            xWriteCH376Data( 0 );
            s = CH376SendCmdWaitInt( CMD0H_FILE_OPEN );
            if ( s == USB_INT_SUCCESS )
            {
                s = CH376ReadVar8( 0xCF );
                if ( s )
                {
                    CH376WriteVar32( 0x4C, CH376ReadVar32( 0x4C ) + ( (u16)s << 8 ) );
                    CH376WriteVar32( 0x50, CH376ReadVar32( 0x50 ) + ( (u16)s << 8 ) );
                    CH376WriteVar32( 0x70, 0 );
                }
            }
        }
    }
#endif
    xWriteCH376Cmd( CMD10_SET_FILE_NAME );
    c = *name;
    xWriteCH376Data( c );
    while ( c )
    {
        name ++;
        c = *name;
        if ( c == DEF_SEPAR_CHAR1 || c == DEF_SEPAR_CHAR2 )
        {
            c = 0;                                                                      /* 强行将文件名截止 */
        }
        xWriteCH376Data( c );
    }
    xEndCH376Cmd( );
}

/*******************************************************************************
* 函  数  名      : CH376Read32bitDat
* 描      述      : 从CH376芯片读取32位的数据并结束命令.
* 输      入      : 无.
* 返      回      : 32位数据.
*******************************************************************************/
u32 CH376Read32bitDat( void )
{
    u8  c0, c1, c2, c3;

    c0 = xReadCH376Data( );
    c1 = xReadCH376Data( );
    c2 = xReadCH376Data( );
    c3 = xReadCH376Data( );
    xEndCH376Cmd( );
    return( c0 | (u16)c1 << 8 | (u32)c2 << 16 | (u32)c3 << 24 );
}

/*******************************************************************************
* 函  数  名      : CH376ReadVar8
* 描      述      : 读CH376芯片内部的8位变量.
* 输      入      : 无.
* 返      回      : 8位变量.
*******************************************************************************/
u8  CH376ReadVar8( u8 var )
{
    u8  c0;

    xWriteCH376Cmd( CMD11_READ_VAR8 );                                                   /* 读取指定的8位文件系统变量 */
    xWriteCH376Data( var );
    c0 = xReadCH376Data( );
    xEndCH376Cmd( );
    return( c0 );
}

/*******************************************************************************
* 函  数  名      : CH376WriteVar8
* 描      述      : 写CH376芯片内部的8位变量.
* 输      入      : u8 var：
*                   变量地址.
*                   u8 dat:
                    数据.
* 返      回      : 无.
*******************************************************************************/
void    CH376WriteVar8( u8 var, u8 dat )
{
    xWriteCH376Cmd( CMD20_WRITE_VAR8 );                                                 /* 设置指定的8位文件系统变量 */
    xWriteCH376Data( var );
    xWriteCH376Data( dat );
    xEndCH376Cmd( );
}

/*******************************************************************************
* 函  数  名      : CH376ReadVar8
* 描      述      : 读CH376芯片内部的32位变量.
* 输      入      : u8 var：
*                   变量地址.
* 返      回      : 32位变量.
*******************************************************************************/
u32 CH376ReadVar32( u8 var )
{
    xWriteCH376Cmd( CMD14_READ_VAR32 );
    xWriteCH376Data( var );
    return( CH376Read32bitDat( ) );                                                     /* 从CH376芯片读取32位的数据并结束命令 */
}

/*******************************************************************************
* 函  数  名      : CH376WriteVar32
* 描      述      : 写CH376芯片内部的32位变量.
* 输      入      : u8 var：
*                   变量地址.
*                   u32 dat:
*                   数据.
* 返      回      : 无.
*******************************************************************************/
void    CH376WriteVar32( u8 var, u32 dat )
{
    xWriteCH376Cmd( CMD50_WRITE_VAR32 );
    xWriteCH376Data( var );
    xWriteCH376Data( (u8)dat );
    xWriteCH376Data( (u8)( (u16)dat >> 8 ) );
    xWriteCH376Data( (u8)( dat >> 16 ) );
    xWriteCH376Data( (u8)( dat >> 24 ) );
    xEndCH376Cmd( );
}

/*******************************************************************************
* 函  数  名      : CH376EndDirInfo
* 描      述      : 在调用CH376DirInfoRead获取FAT_DIR_INFO结构之后应该通知CH376结束.
* 输      入      : 无.
* 返      回      : 无.
*******************************************************************************/
void    CH376EndDirInfo( void )
{
    CH376WriteVar8( 0x0D, 0x00 );
}

/*******************************************************************************
* 函  数  名      : CH376GetFileSize
* 描      述      : 读取当前文件长度.
* 输      入      : 无.
* 返      回      : 文件长度.
*******************************************************************************/
u32 CH376GetFileSize( void )
{
    return( CH376ReadVar32( VAR_FILE_SIZE ) );
}

/*******************************************************************************
* 函  数  名      : CH376GetDiskStatus
* 描      述      : 获取磁盘和文件系统的工作状态.
* 输      入      : 无.
* 返      回      : 状态.
*******************************************************************************/
u8  CH376GetDiskStatus( void )
{
    return( CH376ReadVar8( VAR_DISK_STATUS ) );
}

/*******************************************************************************
* 函  数  名      : CH376GetIntStatus
* 描      述      : 获取中断状态并取消中断请求.
* 输      入      : 无.
* 返      回      : u8 s:
*                   中断状态.
*******************************************************************************/
u8  CH376GetIntStatus( void )
{
    u8  s;

    xWriteCH376Cmd( CMD01_GET_STATUS );
    Delay(50);
    s = xReadCH376Data( );
    xEndCH376Cmd( );
    return( s );
}

/*******************************************************************************
* 函  数  名      : Wait376Interrupt
* 描      述      : 等待CH376中断(INT#低电平)，返回中断状态码, 超时则返回
*                   ERR_USB_UNKNOWN.
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
#ifndef NO_DEFAULT_CH376_INT
u8  Wait376Interrupt( void )
{
#ifdef  DEF_INT_TIMEOUT                                                                 /* 是否定义了超时时间 */
#if     DEF_INT_TIMEOUT < 1                                                             /* 没有定义 */
    while ( Query376Interrupt( ) == FALSE );                                            /* 一直等中断 */
    return( CH376GetIntStatus( ) );                                                     /* 检测到中断 */
#else                                                                                   /* 定义了超时时间 */
    u32 i;

    for ( i = 0; i < DEF_INT_TIMEOUT; i ++ )                                            /* 计数防止超时 */
    {
        if ( Query376Interrupt( ) )
        {
            return( CH376GetIntStatus( ) );                                             /* 检测到中断 */
        }
        /* 在等待CH376中断的过程中,可以做些需要及时处理的其它事情 */
    }
    return( ERR_USB_UNKNOWN );                                                          /* 不应该发生的情况 */
#endif
#else
    u32 i;

    for ( i = 0; i < 5000000; i ++ )                                                    /* 计数防止超时,默认的超时时间,与单片机主频有关 */
    {
        if ( Query376Interrupt( ) )
        {
            return( CH376GetIntStatus( ) );                                             /* 检测到中断 */
        }
        /* 在等待CH376中断的过程中,可以做些需要及时处理的其它事情 */
    }
    return( ERR_USB_UNKNOWN );                                                          /* 不应该发生的情况 */
#endif
}
#endif

/*******************************************************************************
* 函  数  名      : CH376SendCmdWaitInt
* 描      述      : 发出命令码后,等待中断.
* 输      入      : u8 mCmd:
*                   命令码.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376SendCmdWaitInt( u8 mCmd )
{
    xWriteCH376Cmd( mCmd );
    xEndCH376Cmd( );
    return( Wait376Interrupt( ) );
}

/*******************************************************************************
* 函  数  名      : CH376SendCmdDatWaitInt
* 描      述      : 发出命令码和一字节数据后,等待中断.
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376SendCmdDatWaitInt( u8 mCmd, u8 mDat )
{
    xWriteCH376Cmd( mCmd );
    xWriteCH376Data( mDat );
    xEndCH376Cmd( );
    return( Wait376Interrupt( ) );
}

/*******************************************************************************
* 函  数  名      : CH376DiskReqSense
* 描      述      : 检查USB存储器错误.
* 输      入      : 无.
* 返      回      : u8 s:
*                   状态.
*******************************************************************************/
u8  CH376DiskReqSense( void )
{
    u8  s;

    Delay( 5 );
    s = CH376SendCmdWaitInt( CMD0H_DISK_R_SENSE );
    Delay( 5 );
    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376DiskConnect
* 描      述      : 检查U盘是否连接,不支持SD卡.
* 输      入      : 无.
* 返      回      : U盘是否连接状态.
*******************************************************************************/
u8  CH376DiskConnect( void )
{
    if ( Query376Interrupt( ) )
    {
        CH376GetIntStatus( );                                                           /* 检测到中断 */
    }
    return( CH376SendCmdWaitInt( CMD0H_DISK_CONNECT ) );                                /* 检查磁盘是否连接 */
}

/*******************************************************************************
* 函  数  名      : CH376DiskMount
* 描      述      : 初始化磁盘并测试磁盘是否就绪.
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8 CH376DiskMount( void )
{
    return( CH376SendCmdWaitInt( CMD0H_DISK_MOUNT ) );                                  /* 初始化磁盘并测试磁盘是否就绪 */
}

/*******************************************************************************
* 函  数  名      : CH376FileOpen
* 描      述      : 在根目录或者当前目录下打开文件或者目录(文件夹).
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376FileOpen( u8* name )
{
    CH376SetFileName( name );                                                           /* 设置将要操作的文件的文件名 */
#ifndef DEF_IC_V43_U
    if ( name[0] == DEF_SEPAR_CHAR1 || name[0] == DEF_SEPAR_CHAR2 )
    {
        CH376WriteVar32( VAR_CURRENT_CLUST, 0 );
    }
#endif
    return( CH376SendCmdWaitInt( CMD0H_FILE_OPEN ) );
}

/*******************************************************************************
* 函  数  名      : CH376FileCreate
* 描      述      : 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除.
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376FileCreate( u8* name )
{
    if ( name )
    {
        CH376SetFileName( name );   /* 设置将要操作的文件的文件名 */
    }
    return( CH376SendCmdWaitInt( CMD0H_FILE_CREATE ) );
}

/*******************************************************************************
* 函  数  名      : CH376DirCreate
* 描      述      : 在根目录下新建目录(文件夹)并打开,如果目录已经存在那么直接打开.
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376DirCreate( u8* name )
{
    CH376SetFileName( name );   /* 设置将要操作的文件的文件名 */
#ifndef DEF_IC_V43_U
    if ( name[0] == DEF_SEPAR_CHAR1 || name[0] == DEF_SEPAR_CHAR2 )
    {
        CH376WriteVar32( VAR_CURRENT_CLUST, 0 );
    }
#endif
    return( CH376SendCmdWaitInt( CMD0H_DIR_CREATE ) );
}

/*******************************************************************************
* 函  数  名      : CH376SeparatePath
* 描      述      : 从路径中分离出最后一级文件名或者目录(文件夹)名
* 输      入      : u8* path:
*                   指向路径缓冲区.
* 返      回      : 返回最后一级文件名或者目录名的字节偏移.
*******************************************************************************/
u8  CH376SeparatePath( u8* path )
{
    u8* pName;

    for ( pName = path; *pName != 0; ++ pName );                                        /* 到文件名字符串结束位置 */
    while ( *pName != DEF_SEPAR_CHAR1 && *pName != DEF_SEPAR_CHAR2 && pName != path )
    {
        pName --;                                                                       /*  搜索倒数第一个路径分隔符 */
    }
    if ( pName != path )
    {
        pName ++;                                                                       /* 找到了路径分隔符,则修改指向目标文件的最后一级文件名,跳过前面的多级目录名及路径分隔符 */
    }
    return( pName - path );
}

/*******************************************************************************
* 函  数  名      : CH376FileOpenDir
* 描      述      : 打开多级目录下的文件或者目录的上级目录,支持多级目录路径,
*                   支持路径分隔符,路径长度不超过255个字符
* 输      入      : u8* path:
*                   指向路径缓冲区.
*                   u8 StopName:
*                   指向最后一级文件名或者目录名
* 返      回      : 返回最后一级文件名或者目录名的字节偏移.
*******************************************************************************/
u8  CH376FileOpenDir( u8* PathName, u8 StopName )
{
    u8  i, s;

    s = 0;
    i = 1;                                                                              /* 跳过有可能的根目录符 */
    while ( 1 )
    {
        while ( PathName[i] != DEF_SEPAR_CHAR1 && PathName[i] != DEF_SEPAR_CHAR2 && PathName[i] != 0 )
        {
            ++ i;                                                                       /* 搜索下一个路径分隔符或者路径结束符 */
        }

        if ( PathName[i] )
        {
            i ++;                                                                       /* 找到了路径分隔符,修改指向目标文件的最后一级文件名 */
        }
        else
        {
            i = 0;                                                                      /* 路径结束 */
        }

        s = CH376FileOpen( &PathName[s] );                                              /* 打开文件或者目录 */

        if ( i && i != StopName )                                                       /* 路径尚未结束 */
        {
            if ( s != ERR_OPEN_DIR )                                                    /* 因为是逐级打开,尚未到路径结束,所以,如果不是成功打开了目录,那么说明有问题 */
            {
                if ( s == USB_INT_SUCCESS )
                {
                    return( ERR_FOUND_NAME );                                           /* 中间路径必须是目录名,如果是文件名则出错 */
                }
                else if ( s == ERR_MISS_FILE )
                {
                    return( ERR_MISS_DIR );                                             /* 中间路径的某个子目录没有找到,可能是目录名称错误 */
                }
                else
                {
                    return( s );                                                        /* 操作出错 */
                }
            }
            s = i;                                                                      /* 从下一级目录开始继续 */
        }
        else
        {
            return( s );                                                                /* 路径结束,USB_INT_SUCCESS为成功打开文件,ERR_OPEN_DIR为成功打开目录(文件夹),其它为操作出错 */
        }
    }
}

/*******************************************************************************
* 函  数  名      : CH376FileOpenPath
* 描      述      : 打开多级目录下的文件或者目录(文件夹),支持多级目录路径,
*                   支持路径分隔符,路径长度不超过255个字符
* 输      入      : u8* path:
*                   指向路径缓冲区.
* 返      回      : 返回最后一级文件名或者目录名的字节偏移.
*******************************************************************************/
u8  CH376FileOpenPath( u8* PathName )
{
    return( CH376FileOpenDir( PathName, 0xFF ) );
}

/*******************************************************************************
* 函  数  名      : CH376FileCreatePath
* 描      述      : 新建多级目录下的目录(文件夹)并打开,支持多级目录路径,支持路
*                   径分隔符,路径长度不超过255个字符.
* 输      入      : u8* path:
*                   指向路径缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376FileCreatePath( u8* PathName )
{
    u8  s;
    u8  Name;

    Name = CH376SeparatePath( PathName );                                               /* 从路径中分离出最后一级文件名,返回最后一级文件名的偏移 */
    if ( Name )                                                                         /* 是多级目录 */
    {
        s = CH376FileOpenDir( PathName, Name );                                         /* 打开多级目录下的最后一级目录,即打开新建文件的上级目录 */
        if ( s != ERR_OPEN_DIR )                                                        /* 因为是打开上级目录,所以,如果不是成功打开了目录,那么说明有问题 */
        {
            if ( s == USB_INT_SUCCESS )
            {
                return( ERR_FOUND_NAME );                                               /* 中间路径必须是目录名,如果是文件名则出错 */
            }
            else if ( s == ERR_MISS_FILE )
            {
                return( ERR_MISS_DIR );                                                 /* 中间路径的某个子目录没有找到,可能是目录名称错误 */
            }
            else
            {
                return( s );                                                            /* 操作出错 */
            }
        }
    }
    return( CH376FileCreate( &PathName[Name] ) );                                       /* 在根目录或者当前目录下新建文件 */
}

/*******************************************************************************
* 函  数  名      : CH376FileCreatePath
* 描      述      : 新建多级目录下的文件,支持多级目录路径,支持路径分隔符,路径长
*                   度不超过255个字符
* 输      入      : u8* path:   指向路径缓冲区.
* 返      回      : 操作状态.
*******************************************************************************/
#ifdef  EN_DIR_CREATE
u8  CH376DirCreatePath( u8* PathName )
{
    u8  s;
    u8  Name;
    u8  ClustBuf[4];

    Name = CH376SeparatePath( PathName );                                               /* 从路径中分离出最后一级目录名,返回最后一级文件名的偏移 */
    if ( Name )                                                                         /* 是多级目录 */
    {
        s = CH376FileOpenDir( PathName, Name );                                         /* 打开多级目录下的最后一级目录,即打开新建目录的上级目录 */
        if ( s != ERR_OPEN_DIR )                                                        /* 因为是打开上级目录,所以,如果不是成功打开了目录,那么说明有问题 */
        {
            if ( s == USB_INT_SUCCESS )
            {
                return( ERR_FOUND_NAME );                                               /* 中间路径必须是目录名,如果是文件名则出错 */
            }
            else if ( s == ERR_MISS_FILE )
            {
                return( ERR_MISS_DIR );                                                 /* 中间路径的某个子目录没有找到,可能是目录名称错误 */
            }
            else
            {
                return( s );                                                            /* 操作出错 */
            }
        }
        xWriteCH376Cmd( CMD14_READ_VAR32 );
        xWriteCH376Data( VAR_START_CLUSTER );                                           /* 上级目录的起始簇号 */

        for ( s = 0; s != 4; s ++ )
        {
            ClustBuf[ s ] = xReadCH376Data( );
        }

        xEndCH376Cmd( );

        s = CH376DirCreate( &PathName[Name] );                                          /* 在当前目录下新建目录 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        s = CH376ByteLocate( sizeof(FAT_DIR_INFO) + STRUCT_OFFSET( FAT_DIR_INFO, DIR_FstClusHI ) );  /* 移动文件指针 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        s = CH376ByteWrite( &ClustBuf[2], 2, NULL );                                    /* 写入上级目录的起始簇号的高16位 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        s = CH376ByteLocate( sizeof(FAT_DIR_INFO) + STRUCT_OFFSET( FAT_DIR_INFO, DIR_FstClusLO ) );  /* 移动文件指针 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        s = CH376ByteWrite( ClustBuf, 2, NULL );                                        /* 写入上级目录的起始簇号的低16位 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        s = CH376ByteLocate( 0 );                                                       /* 移动文件指针,恢复到目录头位置 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }
    }
    else                                                                                /* 不是多级目录 */
    {
        if ( PathName[0] == DEF_SEPAR_CHAR1 || PathName[0] == DEF_SEPAR_CHAR2 )
        {
            return( CH376DirCreate( PathName ) );                                       /* 在根目录下新建目录 */
        }
        else
        {
            return( ERR_MISS_DIR );                                                     /* 必须提供完整路径才能实现在当前目录下新建目录 */
        }
    }
}
#endif

/*******************************************************************************
* 函  数  名      : CH376FileErase
* 描      述      : 删除文件,如果已经打开则直接删除,否则对于文件会先打开再删除,
*                   支持多级目录路径.
* 输      入      : u8* path:
*                   指向路径缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376FileErase( u8* PathName )
{
    u8  s;

    if ( PathName )                                                                     /* 文件尚未打开 */
    {
        for ( s = 1; PathName[s] != DEF_SEPAR_CHAR1 && PathName[s] != DEF_SEPAR_CHAR2 && PathName[s] != 0; ++ s );  /* 搜索下一个路径分隔符或者路径结束符 */
        if ( PathName[s] )                                                              /* 有路径分隔符,是多级目录下的文件或者目录 */
        {
            s = CH376FileOpenPath( PathName );                                          /* 打开多级目录下的文件或者目录 */
            if ( s != USB_INT_SUCCESS && s != ERR_OPEN_DIR )
            {
                return( s );                                                            /* 操作出错 */
            }
        }
        else
        {
            CH376SetFileName( PathName );                                               /* 没有路径分隔符,是根目录或者当前目录下的文件或者目录,设置将要操作的文件的文件名 */
        }
    }
    return( CH376SendCmdWaitInt( CMD0H_FILE_ERASE ) );
}

/*******************************************************************************
* 函  数  名      : CH376FileClose
* 描      述      : 关闭当前已经打开的文件或者目录(文件夹)
* 输      入      : u8* UpdateSz:
*                   是否更新文件长度.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376FileClose( u8 UpdateSz )
{
    return( CH376SendCmdDatWaitInt( CMD1H_FILE_CLOSE, UpdateSz ) );
}

/*******************************************************************************
* 函  数  名      : CH376DirInfoRead
* 描      述      : 读取当前文件的目录信息
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376DirInfoRead( void )
{
    return( CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ, 0xFF ) );
}

/*******************************************************************************
* 函  数  名      : CH376DirInfoSave
* 描      述      : 保存文件的目录信息
* 输      入      : 无.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376DirInfoSave( void )
{
    return( CH376SendCmdWaitInt( CMD0H_DIR_INFO_SAVE ) );
}

/*******************************************************************************
* 函  数  名      : CH376ByteLocate
* 描      述      : 以字节为单位移动当前文件指针
* 输      入      : u32 offset:
*                   指针偏移地址.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376ByteLocate( u32 offset )
{
    xWriteCH376Cmd( CMD4H_BYTE_LOCATE );
    xWriteCH376Data( (u8)offset );
    xWriteCH376Data( (u8)((u16)offset>>8) );
    xWriteCH376Data( (u8)(offset>>16) );
    xWriteCH376Data( (u8)(offset>>24) );
    xEndCH376Cmd( );
    return( Wait376Interrupt( ) );
}

/*******************************************************************************
* 函  数  名      : CH376ByteRead
* 描      述      : 以字节为单位从当前位置读取数据块
* 输      入      : u8* buf:
*                   指向数据缓冲区.
*                   u16 ReqCount：
*                   请求读取的字节数.
*                   u16* RealCount:
*                   实际读取的字节数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376ByteRead( u8* buf, u16 ReqCount, u16* RealCount )
{
    u8  s;

    xWriteCH376Cmd( CMD2H_BYTE_READ );
    xWriteCH376Data( (u8)ReqCount );
    xWriteCH376Data( (u8)(ReqCount>>8) );
    xEndCH376Cmd( );
    if ( RealCount )
    {
        *RealCount = 0;
    }

    while ( 1 )
    {
        s = Wait376Interrupt( );
        if ( s == USB_INT_DISK_READ )                                                   /* 请求数据读出 */
        {
            s = CH376ReadBlock( buf );                                                  /* 从当前主机端点的接收缓冲区读取数据块,返回长度 */
            xWriteCH376Cmd( CMD0H_BYTE_RD_GO );                                         /* 继续读 */
            xEndCH376Cmd( );
            buf += s;
            if ( RealCount )
            {
                *RealCount += s;
            }
        }
        else
        {
            return( s );                                                                /* 错误 */
        }
    }
}

/*******************************************************************************
* 函  数  名      : CH376ByteWrite
* 描      述      : 以字节为单位向当前位置写入数据块.
* 输      入      : u8* buf:
*                   指向外部缓冲区.
*                   u16 ReqCount：
*                   请求写入的字节数.
*                   u16* RealCount:
*                   实际写入的字节数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376ByteWrite( u8* buf, u16 ReqCount, u16* RealCount )
{
    u8  s;

    xWriteCH376Cmd( CMD2H_BYTE_WRITE );
    xWriteCH376Data( (u8)ReqCount );
    xWriteCH376Data( (u8)(ReqCount>>8) );
    xEndCH376Cmd( );
    if ( RealCount )
    {
        *RealCount = 0;
    }

    while ( 1 )
    {
        s = Wait376Interrupt( );
        if ( s == USB_INT_DISK_WRITE )
        {
            s = CH376WriteReqBlock( buf );                                              /* 向内部指定缓冲区写入请求的数据块,返回长度 */
            xWriteCH376Cmd( CMD0H_BYTE_WR_GO );
            xEndCH376Cmd( );
            buf += s;
            if ( RealCount ) *RealCount += s;
        }
        else
        {
            return( s );                                                                /* 错误 */
        }
    }
}

/*******************************************************************************
* 函  数  名      : CH376DiskCapacity
* 描      述      : 查询磁盘物理容量,扇区数.
* 输      入      : u32* DiskCap:
*                   磁盘容量.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376DiskCapacity( u32* DiskCap )
{
    u8  s;

    s = CH376SendCmdWaitInt( CMD0H_DISK_CAPACITY );
    if ( s == USB_INT_SUCCESS )
    {
        /* 参考CH376INC.H文件中CH376_CMD_DATA结构的DiskCapacity */
        xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
        xReadCH376Data( );                                                              /* 长度总是sizeof(CH376_CMD_DATA.DiskCapacity) */
        *DiskCap = CH376Read32bitDat( );                                                /* CH376_CMD_DATA.DiskCapacity.mDiskSizeSec,从CH376芯片读取32位的数据并结束命令 */
    }
    else
    {
        *DiskCap = 0;
    }
    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376DiskQuery
* 描      述      : 查询磁盘剩余空间信息,扇区数.
* 输      入      : u32* DiskFre:
*                   当前逻辑盘的剩余扇区数.
* 返      回      : 中断状态.
*******************************************************************************/
u8   CH376DiskQuery( u32* DiskFre )
{
    u8  s;
    u8  c0, c1, c2, c3;

#ifndef DEF_IC_V43_U
    xWriteCH376Cmd( CMD01_GET_IC_VER );                                                 /* 获取芯片及固件版本 */
    if ( xReadCH376Data( ) < 0x43 )
    {
        if ( CH376ReadVar8( VAR_DISK_STATUS ) >= DEF_DISK_READY )                       /* 获取主机文件模式下的磁盘及文件状态是否已经能够支持 */
        {
            CH376WriteVar8( VAR_DISK_STATUS, DEF_DISK_MOUNTED );
        }
    }
#endif

    s = CH376SendCmdWaitInt( CMD0H_DISK_QUERY );
    if ( s == USB_INT_SUCCESS )
    {
        /* 参考CH376INC.H文件中CH376_CMD_DATA结构的DiskQuery */
        xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
        xReadCH376Data( );                                                              /* 长度总是sizeof(CH376_CMD_DATA.DiskQuery) */
        xReadCH376Data( );                                                              /* CH376_CMD_DATA.DiskQuery.mTotalSector */
        xReadCH376Data( );
        xReadCH376Data( );
        xReadCH376Data( );
        c0 = xReadCH376Data( );                                                         /* CH376_CMD_DATA.DiskQuery.mFreeSector */
        c1 = xReadCH376Data( );
        c2 = xReadCH376Data( );
        c3 = xReadCH376Data( );
        *DiskFre = c0 | (u16)c1 << 8 | (u32)c2 << 16 | (u32)c3 << 24;
        xReadCH376Data( );                                                              /* CH376_CMD_DATA.DiskQuery.mDiskFat */
        xEndCH376Cmd( );
    }
    else
    {
        *DiskFre = 0;
    }

    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376SecLocate
* 描      述      : 以扇区为单位移动当前文件指针.
* 输      入      : u32 offset:
*                   要移动的扇区数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376SecLocate( u32 offset )
{
    xWriteCH376Cmd( CMD4H_SEC_LOCATE );
    xWriteCH376Data( (u8)offset );
    xWriteCH376Data( (u8)((u16)offset>>8) );
    xWriteCH376Data( (u8)(offset>>16) );
    xWriteCH376Data( 0 );                                                               /* 超出最大文件尺寸 */
    xEndCH376Cmd( );
    return( Wait376Interrupt() );
}

/*******************************************************************************
* 函  数  名      : CH376DiskReadSec
* 描      述      : 从U盘读取多个扇区的数据块到缓冲区,不支持SD卡.
* 输      入      : u8* buf:
*                   指向外部数据缓冲区.
*                   u32 iLbaStart：
*                   iLbaStart 是准备读取的线性起始扇区号.
*                   u8 iSectorCount:
*                   iSectorCount 是准备读取的扇区数
* 返      回      : 中断状态.
*******************************************************************************/
#ifdef  EN_SECTOR_ACCESS
u8  CH376DiskReadSec( u8* buf, u32 iLbaStart, u8 iSectorCount )
{
    u8  s, err;
    u16 mBlockCount;

    for ( err = 0; err != 3; ++ err )                                                   /* 出错重试 */
    {
        xWriteCH376Cmd( CMD5H_DISK_READ );                                              /* 从USB存储器读扇区 */
        xWriteCH376Data( (u8)iLbaStart );                                            /* LBA的最低8位 */
        xWriteCH376Data( (u8)( (u16)iLbaStart >> 8 ) );
        xWriteCH376Data( (u8)( iLbaStart >> 16 ) );
        xWriteCH376Data( (u8)( iLbaStart >> 24 ) );                                  /* LBA的最高8位 */
        xWriteCH376Data( iSectorCount );                                                /* 扇区数 */
        xEndCH376Cmd( );
        for ( mBlockCount = iSectorCount * DEF_SECTOR_SIZE / CH376_DAT_BLOCK_LEN; mBlockCount != 0; -- mBlockCount ) /* 数据块计数 */
        {
            s = Wait376Interrupt( );                                                    /* 等待中断并获取状态 */
            if ( s == USB_INT_DISK_READ )                                               /* USB存储器读数据块,请求数据读出 */
            {
                s = CH376ReadBlock( buf );                                              /* 从当前主机端点的接收缓冲区读取数据块,返回长度 */
                xWriteCH376Cmd( CMD0H_DISK_RD_GO );                                     /* 继续执行USB存储器的读操作 */
                xEndCH376Cmd( );
                buf += s;
            }
            else
            {
                break;                                                                  /* 返回错误状态 */
            }
        }

        if ( mBlockCount == 0 )
        {
            s = Wait376Interrupt( );                                                    /* 等待中断并获取状态 */
            if ( s == USB_INT_SUCCESS )
            {
                return( USB_INT_SUCCESS );                                              /* 操作成功 */
            }
        }

        if ( s == USB_INT_DISCONNECT )
        {
            return( s );                                                                /* U盘被移除 */
        }

        CH376DiskReqSense( );                                                           /* 检查USB存储器错误 */
    }

    return( s );                                                                        /* 操作失败 */
}

/*******************************************************************************
* 函  数  名      : CH376DiskWriteSec
* 描      述      : 将缓冲区中的多个扇区的数据块写入U盘,不支持SD卡.
* 输      入      : u8* buf:
*                   指向外部数据缓冲区.
*                   u32 iLbaStart：
*                   写入的线起始性扇区号.
*                   u8 iSectorCount:
*                   写入的扇区数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376DiskWriteSec( u8* buf, u32 iLbaStart, u8 iSectorCount )
{
    u8  s, err;
    u16 mBlockCount;

    for ( err = 0; err != 3; ++ err )                                                   /* 出错重试 */
    {
        xWriteCH376Cmd( CMD5H_DISK_WRITE );                                             /* 向USB存储器写扇区 */
        xWriteCH376Data( (u8)iLbaStart );                                            /* LBA的最低8位 */
        xWriteCH376Data( (u8)( (u16)iLbaStart >> 8 ) );
        xWriteCH376Data( (u8)( iLbaStart >> 16 ) );
        xWriteCH376Data( (u8)( iLbaStart >> 24 ) );                                  /* LBA的最高8位 */
        xWriteCH376Data( iSectorCount );                                                /* 扇区数 */
        xEndCH376Cmd( );
        for ( mBlockCount = iSectorCount * DEF_SECTOR_SIZE / CH376_DAT_BLOCK_LEN; mBlockCount != 0; -- mBlockCount ) /* 数据块计数 */
        {
            s = Wait376Interrupt( );                                                    /* 等待中断并获取状态 */
            if ( s == USB_INT_DISK_WRITE )                                              /* USB存储器写数据块,请求数据写入 */
            {
                CH376WriteHostBlock( buf, CH376_DAT_BLOCK_LEN );                        /* 向USB主机端点的发送缓冲区写入数据块 */
                xWriteCH376Cmd( CMD0H_DISK_WR_GO );                                     /* 继续执行USB存储器的写操作 */
                xEndCH376Cmd( );
                buf += CH376_DAT_BLOCK_LEN;
            }
            else
            {
                break;                                                                  /* 返回错误状态 */
            }
        }

        if ( mBlockCount == 0 )
        {
            s = Wait376Interrupt( );                                                    /* 等待中断并获取状态 */
            if ( s == USB_INT_SUCCESS ) return( USB_INT_SUCCESS );                      /* 操作成功 */
        }

        if ( s == USB_INT_DISCONNECT )
        {
            return( s );                                                                /* U盘被移除 */
        }

        CH376DiskReqSense( );                                                           /* 检查USB存储器错误 */
    }
    return( s );                                                                        /* 操作失败 */
}

/*******************************************************************************
* 函  数  名      : CH376SecRead
* 描      述      : 以扇区为单位从当前位置读取数据块,不支持SD卡.
* 输      入      : u8* buf:
*                   指向外部数据缓冲区.
*                   u8 ReqCount：
*                   请求读出的扇区数.
*                   u8* RealCount:
*                   实际读出的扇区数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376SecRead( u8* buf, u8 ReqCount, u8* RealCount )
{
    u8  s;
    u8  cnt;
    u32 StaSec;

#ifndef DEF_IC_V43_U
    u32 fsz, fofs;
#endif
    if ( RealCount )
    {
        *RealCount = 0;
    }

    do
    {
#ifndef DEF_IC_V43_U
        xWriteCH376Cmd( CMD01_GET_IC_VER );
        cnt = xReadCH376Data( );

        if ( cnt == 0x41 )
        {
            xWriteCH376Cmd( CMD14_READ_VAR32 );
            xWriteCH376Data( VAR_FILE_SIZE );
            xReadCH376Data( );
            fsz = xReadCH376Data( );
            fsz |= (u16)(xReadCH376Data( )) << 8;
            cnt = xReadCH376Data( );
            fsz |= (u32)cnt << 16;

            xWriteCH376Cmd( CMD14_READ_VAR32 );
            xWriteCH376Data( VAR_CURRENT_OFFSET );
            xReadCH376Data( );
            fofs = xReadCH376Data( );
            fofs |= (u16)(xReadCH376Data( )) << 8;
            fofs |= (u32)(xReadCH376Data( )) << 16;

            if ( fsz >= fofs + 510 )
            {
                CH376WriteVar8( VAR_FILE_SIZE + 3, 0xFF );
            }
            else
            {
                cnt = 0xFF;
            }
        }
        else
        {
            cnt = 0xFF;
        }
#endif
        xWriteCH376Cmd( CMD1H_SEC_READ );
        xWriteCH376Data( ReqCount );
        xEndCH376Cmd( );
        s = Wait376Interrupt( );
#ifndef DEF_IC_V43_U
        if ( cnt != 0xFF )
        {
            CH376WriteVar8( VAR_FILE_SIZE + 3, cnt );
        }
#endif
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
        xReadCH376Data( );                                                              /* 长度总是sizeof(CH376_CMD_DATA.SectorRead) */
        cnt = xReadCH376Data( );                                                        /* CH376_CMD_DATA.SectorRead.mSectorCount */
        xReadCH376Data( );
        xReadCH376Data( );
        xReadCH376Data( );
        StaSec = CH376Read32bitDat( );                                                  /* CH376_CMD_DATA.SectorRead.mStartSector,从CH376芯片读取32位的数据并结束命令 */

        if ( cnt == 0 )
        {
            break;
        }

        s = CH376DiskReadSec( buf, StaSec, cnt );                                       /* 从U盘读取多个扇区的数据块到缓冲区 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        buf += cnt * DEF_SECTOR_SIZE;
        if ( RealCount )
        {
            *RealCount += cnt;
        }
        ReqCount -= cnt;

    }
    while ( ReqCount );

    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376SecWrite
* 描      述      : 以扇区为单位在当前位置写入数据块,不支持SD卡.
* 输      入      : u8* buf:
*                   指向外部数据缓冲区.
*                   u8 ReqCount：
*                   请求写入的扇区数.
*                   u8* RealCount:
*                   实际写入的扇区数.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376SecWrite( u8* buf, u8 ReqCount, u8* RealCount )
{
    u8  s;
    u8  cnt;
    u32 StaSec;

    if ( RealCount )
    {
        *RealCount = 0;
    }

    do
    {
        xWriteCH376Cmd( CMD1H_SEC_WRITE );
        xWriteCH376Data( ReqCount );
        xEndCH376Cmd( );
        s = Wait376Interrupt( );
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
        xReadCH376Data( );                                                              /* 长度总是sizeof(CH376_CMD_DATA.SectorWrite) */
        cnt = xReadCH376Data( );                                                        /* CH376_CMD_DATA.SectorWrite.mSectorCount */
        xReadCH376Data( );
        xReadCH376Data( );
        xReadCH376Data( );
        StaSec = CH376Read32bitDat( );                                                  /* CH376_CMD_DATA.SectorWrite.mStartSector,从CH376芯片读取32位的数据并结束命令 */
        if ( cnt == 0 )
        {
            break;
        }
        s = CH376DiskWriteSec( buf, StaSec, cnt );                                      /* 将缓冲区中的多个扇区的数据块写入U盘 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }

        buf += cnt * DEF_SECTOR_SIZE;
        if ( RealCount )
        {
            *RealCount += cnt;
        }

        ReqCount -= cnt;

    }
    while ( ReqCount );
    return( s );
}

#endif

/*******************************************************************************
* 函  数  名      : CH376LongNameWrite
* 描      述      : 长文件名专用的字节写子程序.
* 输      入      : u8* buf:
*                   指向外部数据缓冲区.
*                   u16 ReqCount：
*                   请求写入的字节数.
* 返      回      : 中断状态.
*******************************************************************************/
#ifdef  EN_LONG_NAME
u8  CH376LongNameWrite( u8* buf, u16 ReqCount )
{
    u8  s;
#ifndef DEF_IC_V43_U
    u8  c;

    c = CH376ReadVar8( VAR_DISK_STATUS );
    if ( c == DEF_DISK_OPEN_ROOT )
    {
        CH376WriteVar8( VAR_DISK_STATUS, DEF_DISK_OPEN_DIR );
    }
#endif
    xWriteCH376Cmd( CMD2H_BYTE_WRITE );
    xWriteCH376Data( (u8)ReqCount );
    xWriteCH376Data( (u8)(ReqCount>>8) );
    xEndCH376Cmd( );
    while ( 1 )
    {
        s = Wait376Interrupt( );
        if ( s == USB_INT_DISK_WRITE )
        {
            if ( buf )
            {
                buf += CH376WriteReqBlock( buf );                                       /* 向内部指定缓冲区写入请求的数据块,返回长度 */
            }
            else
            {
                xWriteCH376Cmd( CMD01_WR_REQ_DATA );                                    /* 向内部指定缓冲区写入请求的数据块 */
                s = xReadCH376Data( );                                                  /* 长度 */
                while ( s -- )
                {
                    xWriteCH376Data( 0 );                                               /* 填充0 */
                }
            }

            xWriteCH376Cmd( CMD0H_BYTE_WR_GO );
            xEndCH376Cmd( );
        }
        else
        {
#ifndef DEF_IC_V43_U
            if ( c == DEF_DISK_OPEN_ROOT )
            {
                CH376WriteVar8( VAR_DISK_STATUS, c );
            }
#endif
            return( s );                                                                /* 错误 */
        }
    }
}

/*******************************************************************************
* 函  数  名      : CH376CheckNameSum
* 描      述      : 计算长文件名的短文件名检验和,输入为无小数点分隔符的固定11字
*                   节格式.
* 输      入      : u8* DirName:
*                   指向外部文件名缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376CheckNameSum( u8* DirName )
{
    u8  NameLen;
    u8  CheckSum;

    CheckSum = 0;
    for ( NameLen = 0; NameLen != 11; NameLen ++ )
    {
        CheckSum = ( CheckSum & 1 ? 0x80 : 0x00 ) + ( CheckSum >> 1 ) + *DirName++;
    }
    return( CheckSum );
}

/*******************************************************************************
* 函  数  名      : CH376LocateInUpDir
* 描      述      : 在上级目录(文件夹)中移动文件指针到当前文件目录信息所在的扇区.
*                   另外,顺便将当前文件目录信息所在的扇区的前一个扇区的LBA地址写
*                   入CH376内部VAR_FAT_DIR_LBA变量(为了方便收集长文件名时向前搜索
*                   ,否则要多移动一次.
*                   使用了全局缓冲区GlobalBuf的前12个字节.
* 输      入      : u8* PathName:
*                   指向路径缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376LocateInUpDir( u8* PathName )
{
    u8  s;

    xWriteCH376Cmd( CMD14_READ_VAR32 );
    xWriteCH376Data( VAR_FAT_DIR_LBA );                                                 /* 当前文件目录信息所在的扇区LBA地址 */
    for ( s = 4; s != 8; s ++ )
    {
        GlobalBuf[ s ] = xReadCH376Data( );                                             /* 临时保存于全局缓冲区中,节约RAM */
    }

    xEndCH376Cmd( );

    s = CH376SeparatePath( PathName );                                                  /* 从路径中分离出最后一级文件名或者目录名,返回最后一级文件名或者目录名的偏移 */
    if ( s )
    {
        s = CH376FileOpenDir( PathName, s );                                            /* 是多级目录,打开多级目录下的最后一级目录,即打开文件的上级目录 */
    }
    else
    {
        s = CH376FileOpen( "/" );                                                       /* 根目录下的文件,则打开根目录 */
    }

    if ( s != ERR_OPEN_DIR )
    {
        return( s );
    }
    *(u32*)( &GlobalBuf[0] ) = 0;                                                    /* 目录扇区偏移扇区数,保存在全局缓冲区中,节约RAM */

    while ( 1 )                                                                         /* 不断移动文件指针,直到与当前文件目录信息所在的扇区LBA地址匹配 */
    {
        s = CH376SecLocate( *(u32*)(&GlobalBuf[0]) );                                /* 以扇区为单位在上级目录中移动文件指针 */
        if ( s != USB_INT_SUCCESS )
        {
            return( s );
        }
        CH376ReadBlock( &GlobalBuf[8] );                                                /* 从内存缓冲区读取CH376_CMD_DATA.SectorLocate.mSectorLba数据块,返回长度总是sizeof(CH376_CMD_DATA.SectorLocate) */
        if ( *(u32*)(&GlobalBuf[8]) == *(u32*)(&GlobalBuf[4]) )
        {
            return( USB_INT_SUCCESS );                                                  /* 已到当前文件目录信息扇区 */
        }
        xWriteCH376Cmd( CMD50_WRITE_VAR32 );
        xWriteCH376Data( VAR_FAT_DIR_LBA );                                             /* 得到前一个扇区,设置为新的文件目录信息扇区LBA地址 */
        for ( s = 8; s != 12; s ++ )
        {
            xWriteCH376Data( GlobalBuf[ s ] );
        }
        xEndCH376Cmd( );
        ++ *(u32*)(&GlobalBuf[0]);
    }
}

/*******************************************************************************
* 函  数  名      : CH376CheckNameSum
* 描      述      : 由短文件名或者目录(文件夹)名获得相应的长文件名.
*                   需要输入短文件名的完整路径PathName,需要提供缓冲区接收长文件
*                   名LongName(以UNICODE小端编码,以双0结束),
*                   使用了全局缓冲区GlobalBuf的前34个字节,
*                   sizeof(GlobalBuf) >= sizeof(FAT_DIR_INFO)+2
* 输      入      : u8* PathName:
*                   短文件名的完整路径.
*                   u8* LongName:
*                   指向长文件名接收缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376GetLongName( u8* PathName, u8* LongName )
{
    u8  s;
    u16 NameCount;                                                                  /* 长文件名字节计数 */

    s = CH376FileOpenPath( PathName );                                                  /* 打开多级目录下的文件或者目录 */
    if ( s != USB_INT_SUCCESS && s != ERR_OPEN_DIR )
    {
        return( s );
    }

    s = CH376DirInfoRead( );                                                            /* 读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中 */
    if ( s != USB_INT_SUCCESS )
    {
        return( s );
    }

    CH376ReadBlock( GlobalBuf );                                                        /* 从内存缓冲区读取FAT_DIR_INFO数据块,返回长度总是sizeof(FAT_DIR_INFO) */
    CH376EndDirInfo( );                                                                 /* 获取完FAT_DIR_INFO结构 */
    GlobalBuf[32] = CH376CheckNameSum( GlobalBuf );                                     /* 计算长文件名的短文件名检验和,保存在全局缓冲区中,节约RAM */
    GlobalBuf[33] = CH376ReadVar8( VAR_FILE_DIR_INDEX );                                /* 当前文件目录信息在扇区内的索引号,保存在全局缓冲区中,节约RAM */
    NameCount = 0;
    while ( 1 )
    {
        if ( GlobalBuf[33] == 0 )                                                       /* 当前的文件目录信息扇区处理结束,转到前一个扇区 */
        {
            s = CH376LocateInUpDir( PathName );                                         /* 在上级目录中移动文件指针到当前文件目录信息所在的扇区 */
            if ( s != USB_INT_SUCCESS )
            {
                break;
            }

            if ( CH376ReadVar32( VAR_CURRENT_OFFSET ) == 0 )                            /* 当前已经处于目录扇区的开始,无法获取长文件名 */
            {
                s = ERR_LONG_NAME_ERR;
                break;
            }
            GlobalBuf[33] = DEF_SECTOR_SIZE / sizeof( FAT_DIR_INFO );                   /* 指向前一个扇区的最后一个文件目录信息 */
        }

        GlobalBuf[33] --;                                                               /* 从后向前搜索文件目录信息 */
        s = CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ, GlobalBuf[33] );               /* 读取指定的目录信息FAT_DIR_INFO,将相关数据调到内存中 */
        if ( s != USB_INT_SUCCESS )
        {
            break;
        }

        CH376ReadBlock( GlobalBuf );                                                    /* 从内存缓冲区读取FAT_DIR_INFO数据块,返回长度总是sizeof(FAT_DIR_INFO) */
        CH376EndDirInfo( );                                                             /* 获取完FAT_DIR_INFO结构 */
        if ( ( GlobalBuf[11] & ATTR_LONG_NAME_MASK ) != ATTR_LONG_NAME || GlobalBuf[13] != GlobalBuf[32] ) /* 类型错误或者校验和错误 */
        {
            s = ERR_LONG_NAME_ERR;
            break;                                                                      /* 没有直接返回是因为如果是打开了根目录那么必须要关闭后才能返回 */
        }

        for ( s = 1; s < sizeof( FAT_DIR_INFO ); s += 2 )                               /* 收集长文件名,长文件名的字符在磁盘上UNICODE用小端方式存放 */
        {
            if ( s == 1 + 5 * 2 )
            {
                s = 14;                                                                 /* 从长文件名的第一组1-5个字符跳到第二组6-11个字符 */
            }
            else if ( s == 14 + 6 * 2 )
            {
                s = 28;                                                                 /* 从长文件名的第二组6-11个字符跳到第三组12-13个字符 */
            }

            LongName[ NameCount++ ] = GlobalBuf[ s ];
            LongName[ NameCount++ ] = GlobalBuf[ s + 1 ];
            if ( GlobalBuf[ s ] == 0 && GlobalBuf[ s + 1 ] == 0 )
            {
                break;                                                                  /* 长文件名结束 */
            }

            if ( NameCount >= LONG_NAME_BUF_LEN )                                       /* 长文件名缓冲区溢出 */
            {
                s = ERR_LONG_BUF_OVER;
                goto CH376GetLongNameE;
            }
        }

        if ( GlobalBuf[0] & 0x40 )                                                      /* 长文件名目录信息块结束 */
        {
            if ( s >= sizeof( FAT_DIR_INFO ) ) *(u16*)( &LongName[ NameCount ] ) = 0x0000;  /* 尚未收集到长文件名的结束符,则强制结束 */
            s = USB_INT_SUCCESS;                                                        /* 成功完成长文件名收集完成 */
            break;
        }
    }

CH376GetLongNameE:
    CH376FileClose( FALSE );                                                            /* 对于根目录则必须要关闭 */
    return( s );
}

/*******************************************************************************
* 函  数  名      : CH376CreateLongName
* 描      述      : 新建具有长文件名的文件,关闭文件后返回,LongName输入路径必须
*                   在RAM中.
*                   需要输入短文件名的完整路径PathName(请事先参考FAT规范由长文
*                   件名自行产生),需要输入以UNICODE小端编码的以双0结束的长文件
*                   名LongName.
*                   使用了全局缓冲区GlobalBuf的前64个字节,
*                   sizeof(GlobalBuf)>=sizeof(FAT_DIR_INFO)*2
* 输      入      : u8* PathName:
*                   短文件名的完整路径.
*                   u8* LongName:
*                   指向长文件名缓冲区.
* 返      回      : 中断状态.
*******************************************************************************/
u8  CH376CreateLongName( u8* PathName, u8* LongName )
{
    u8  s, i;
    u8  DirBlockCnt;                                                                /* 长文件名占用文件目录结构的个数 */
    u16 count;                                                                      /* 临时变量,用于计数,用于字节读文件方式下实际读取的字节数 */
    u16 NameCount;                                                                  /* 长文件名字节计数 */
    u32 NewFileLoc;                                                                 /* 当前文件目录信息在上级目录中的起始位置,偏移地址 */

    for ( count = 0; count < LONG_NAME_BUF_LEN; count += 2 )
    {
        if ( *(u16*)(&LongName[count]) == 0 )
        {
            break;                                                                      /* 到结束位置 */
        }
    }

    if ( count == 0 || count >= LONG_NAME_BUF_LEN || count > LONE_NAME_MAX_CHAR )
    {
        return( ERR_LONG_NAME_ERR );                                                    /* 长文件名无效 */
    }

    DirBlockCnt = count / LONG_NAME_PER_DIR;                                            /* 长文件名占用文件目录结构的个数 */
    i = count - DirBlockCnt * LONG_NAME_PER_DIR;

    if ( i )                                                                            /* 有零头 */
    {
        if ( ++ DirBlockCnt * LONG_NAME_PER_DIR > LONG_NAME_BUF_LEN )
        {
            return( ERR_LONG_BUF_OVER );                                                /* 缓冲区溢出 */
        }
        count += 2;                                                                     /* 加上0结束符后的长度 */
        i += 2;

        if ( i < LONG_NAME_PER_DIR )                                                    /* 最末的文件目录结构不满 */
        {
            while ( i++ < LONG_NAME_PER_DIR )
            {
                LongName[count++] = 0xFF;                                               /* 把剩余数据填为0xFF */
            }
        }
    }

    s = CH376FileOpenPath( PathName );                                                  /* 打开多级目录下的文件 */
    if ( s == USB_INT_SUCCESS )                                                         /* 短文件名存在则返回错误 */
    {
        s = ERR_NAME_EXIST;
        goto CH376CreateLongNameE;
    }

    if ( s != ERR_MISS_FILE )
    {
        return( s );
    }

    s = CH376FileCreatePath( PathName );                                                /* 新建多级目录下的文件 */
    if ( s != USB_INT_SUCCESS )
    {
        return( s );
    }

    i = CH376ReadVar8( VAR_FILE_DIR_INDEX );                                            /* 临时用于保存当前文件目录信息在扇区内的索引号 */
    s = CH376LocateInUpDir( PathName );                                                 /* 在上级目录中移动文件指针到当前文件目录信息所在的扇区 */
    if ( s != USB_INT_SUCCESS )
    {
        goto CH376CreateLongNameE;                                                      /* 没有直接返回是因为如果是打开了根目录那么必须要关闭后才能返回 */
    }

    NewFileLoc = CH376ReadVar32( VAR_CURRENT_OFFSET ) + i * sizeof(FAT_DIR_INFO);       /* 计算当前文件目录信息在上级目录中的起始位置,偏移地址 */
    s = CH376ByteLocate( NewFileLoc );                                                  /* 在上级目录中移动文件指针到当前文件目录信息的位置 */
    if ( s != USB_INT_SUCCESS )
    {
        goto CH376CreateLongNameE;
    }

    s = CH376ByteRead( &GlobalBuf[ sizeof(FAT_DIR_INFO) ], sizeof(FAT_DIR_INFO), NULL );/* 以字节为单位读取数据,获得当前文件的目录信息FAT_DIR_INFO */
    if ( s != USB_INT_SUCCESS )
    {
        goto CH376CreateLongNameE;
    }

    for ( i = DirBlockCnt; i != 0; -- i )                                               /* 搜索空闲的文件目录结构用于存放长文件名 */
    {
        s = CH376ByteRead( GlobalBuf, sizeof(FAT_DIR_INFO), &count );                   /* 以字节为单位读取数据,获得下一个文件目录信息FAT_DIR_INFO */
        if ( s != USB_INT_SUCCESS )
        {
            goto CH376CreateLongNameE;
        }

        if ( count == 0 )
        {
            break;                                                                      /* 无法读出数据,上级目录结束了 */
        }

        if ( GlobalBuf[0] && GlobalBuf[0] != 0xE5 )                                     /* 后面有正在使用的文件目录结构,由于长文件名必须连接存放,所以空间不够,必须放弃当前位置并向后转移 */
        {
            s = CH376ByteLocate( NewFileLoc );                                          /* 在上级目录中移动文件指针到当前文件目录信息的位置 */
            if ( s != USB_INT_SUCCESS )
            {
                goto CH376CreateLongNameE;
            }

            GlobalBuf[ 0 ] = 0xE5;                                                      /* 文件删除标志 */
            for ( s = 1; s != sizeof(FAT_DIR_INFO); s ++ )
            {
                GlobalBuf[ s ] = GlobalBuf[ sizeof(FAT_DIR_INFO) + s ];
            }

            s = CH376LongNameWrite( GlobalBuf, sizeof(FAT_DIR_INFO) );                  /* 写入一个文件目录结构,用于删除之前新建的文件,实际上稍后会将之转移到目录的最末位置 */
            if ( s != USB_INT_SUCCESS )
            {
                goto CH376CreateLongNameE;
            }

            do                                                                          /* 向后搜索空闲的文件目录结构 */s
            {
                s = CH376ByteRead( GlobalBuf, sizeof(FAT_DIR_INFO), &count );           /* 以字节为单位读取数据,获得下一个文件目录信息FAT_DIR_INFO */
                if ( s != USB_INT_SUCCESS )
                {
                    goto CH376CreateLongNameE;
                }
            }
            while ( count && GlobalBuf[0] );                                            /* 如果仍然是正在使用的文件目录结构则继续向后搜索,直到上级目录结束或者有尚未使用过的文件目录结构 */

            NewFileLoc = CH376ReadVar32( VAR_CURRENT_OFFSET );                          /* 用上级目录的当前文件指针作为当前文件目录信息在上级目录中的起始位置 */
            i = DirBlockCnt + 1;                                                        /* 需要的空闲的文件目录结构的个数,包括短文件名本身一个和长文件名 */

            if ( count == 0 )
            {
                break;                                                                  /* 无法读出数据,上级目录结束了 */
            }

            NewFileLoc -= sizeof(FAT_DIR_INFO);                                         /* 倒回到刚才搜索到的空闲的文件目录结构的起始位置 */
        }
    }

    if ( i )                                                                            /* 空闲的文件目录结构不足以存放长文件名,原因是上级目录结束了,下面增加上级目录的长度 */
    {
        s = CH376ReadVar8( VAR_SEC_PER_CLUS );                                          /* 每簇扇区数 */
        if ( s == 128 )                                                                 /* FAT12/FAT16的根目录,容量是固定的,无法增加文件目录结构 */
        {
            s = ERR_FDT_OVER;                                                           /* FAT12/FAT16根目录下的文件数应该少于512个,需要磁盘整理 */
            goto CH376CreateLongNameE;
        }

        count = s * DEF_SECTOR_SIZE;                                                    /* 每簇字节数 */
        if ( count < i * sizeof(FAT_DIR_INFO) )
        {
            count <<= 1;                                                                /* 一簇不够则增加一簇,这种情况只会发生于每簇为512字节的情况下 */
        }

        s = CH376LongNameWrite( NULL, count );                                          /* 以字节为单位向当前位置写入全0数据块,清空新增加的文件目录簇 */
        if ( s != USB_INT_SUCCESS )
        {
            goto CH376CreateLongNameE;
        }
    }

    s = CH376ByteLocate( NewFileLoc );                                                  /* 在上级目录中移动文件指针到当前文件目录信息的位置 */
    if ( s != USB_INT_SUCCESS )
    {
        goto CH376CreateLongNameE;
    }

    GlobalBuf[11] = ATTR_LONG_NAME;
    GlobalBuf[12] = 0x00;
    GlobalBuf[13] = CH376CheckNameSum( &GlobalBuf[ sizeof(FAT_DIR_INFO) ] );            /* 计算长文件名的短文件名检验和 */
    GlobalBuf[26] = 0x00;
    GlobalBuf[27] = 0x00;

    for ( s = 0; DirBlockCnt != 0; )                                                    /* 长文件名占用的文件目录结构计数 */
    {
        GlobalBuf[0] = s ? DirBlockCnt : DirBlockCnt | 0x40;                            /* 首次要置长文件名入口标志 */
        DirBlockCnt --;
        NameCount = DirBlockCnt * LONG_NAME_PER_DIR;

        for ( s = 1; s < sizeof( FAT_DIR_INFO ); s += 2 )                               /* 输出长文件名,长文件名的字符在磁盘上UNICODE用小端方式存放 */
        {
            if ( s == 1 + 5 * 2 )
            {
                s = 14;                                                                 /* 从长文件名的第一组1-5个字符跳到第二组6-11个字符 */
            }
            else if ( s == 14 + 6 * 2 )
            {
                s = 28;                                                                 /* 从长文件名的第二组6-11个字符跳到第三组12-13个字符 */
            }
            GlobalBuf[ s ] = LongName[ NameCount++ ];
            GlobalBuf[ s + 1 ] = LongName[ NameCount++ ];
        }
        s = CH376LongNameWrite( GlobalBuf, sizeof(FAT_DIR_INFO) );                      /* 以字节为单位写入一个文件目录结构,长文件名 */
        if ( s != USB_INT_SUCCESS )
        {
            goto CH376CreateLongNameE;
        }
    }

    s = CH376LongNameWrite( &GlobalBuf[ sizeof(FAT_DIR_INFO) ], sizeof(FAT_DIR_INFO) ); /* 以字节为单位写入一个文件目录结构,这是转移来的之前新建的文件的目录信息 */

CH376CreateLongNameE:
    CH376FileClose( FALSE );                                                            /* 对于根目录则必须要关闭 */
    return( s );
}
#endif
/************************************ End *************************************/
