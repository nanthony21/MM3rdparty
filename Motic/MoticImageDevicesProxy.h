/**
 * @file MoticImageDeviceProxy.h
 *
 * @brief MoticImageDevice成像设备模块代理接口是一个C语言动态库(dll)
 *        the proxy of Motic Capture module(MoticImageDevice) is a C dynamic library(dll). 
 *        用户可通过该代理动态库，打开成像模块以直接获取图像集成到他们的应用程序中，并间接查寻与操作Motic设备
 *        The developer, who want to integrate the motic device's image into his application, can use this dll query and operate the Motic device indirectly
 *        该动态库为第三方软件与Motic的成像模块（MoticImageDevice)建立连接，使第三方软件很容易地集成motic设备
 *        this dynamic library create a bridge between the 3rd party application and Motic capture module(MoticImageDevice), allow the 3rd party application use Motic's Camera easily.
 *
 * @code
 * void Open()
 * {
 *    if(MIDP_Open(false) == 0)//打开，Open Proxy
 *    {
 *      int count = MIDP_GetCameraCount();//查寻设备数，query camera count
 *      for(int i = 0; i < count; i++)
 *      {
 *        AddCameraName(MIDP_GetCameraName(i));//保存设备名，store the name of camera
 *      }    
 *      SetCurCameraIndex(MIDP_GetCurCameraIndex());//选择当前打开的设备，select current open camera
 *      count = MIDP_GetResolutionCount(); //查寻分辨率个数，query the count of resolution
 *      for (int i = 0; i < count; i++)
 *      {
 *        CString strFmt;
 *        long w, h;
 *        MIDP_GetResolution(i, &w, &h);//查寻分辨率，query the resolution
 *        strFmt.Format(_T("%dX%d"), w, h);
 *        AddCurrentResolution(strFmt);//保存分辨率，store the resolution
 *      }
 *     SetCurResolutionIndex(MIDP_GetCurResolutionIndex()); //选择当前分辨率，select current resolution  
 *    }
 * }
 *
 * void GetFrame()
 * {
 *   long w, h, bits, channels;
 *   MIDP_GetFormat(&w, &h, &bits, &channels);
 *   int sz = w*h*bits*channels/8;
 *   //extend unsigned char* m_pBuffer; int m_iSize;
 *   if(m_pBuffer== NULL || sz > m_iSize)
 *   {
 *     m_iSize = sz;
 *     if(m_pBuffer)
 *     {
 *       delete[] m_pBuffer;
 *       m_pBuffer = 0;
 *     }
 *     m_pBuffer = new unsigned char[m_iSize];
 *   }
 *   if(MIDP_GetFrame(m_pBuffer, w, h) == 0)
 *   {
 *     ShowImage(m_pBuffer, w, h);
 *   }
 * }
 * 
 * void Show(bool bShow)
 * {
 *   if(bShow) 
 *     MIDP_Show(1);
 *   else
 *     MIDP_Show(0);
 * }
 *
 * @endcode
 *
 * @Author: Chendemin@motic.com
 *
 * @Date:2011-9-19
 *
 */
#pragma once
/**
 * @brief 打开Motic Image Devices 模块
 *        Open Motic Image Devices Modules
 *
 * @param bShow 初始化后是否显示MoticImageDevices界面，bShow:0不显示,其他显示
 *              display or hide the MoticImageDevices window after open, 0, hide, otherwise display 
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
*/
long MIDP_Open(long bShow);

/**
 * @brief 　关闭MoticImageDevices，退出进程前需调用关闭 　
 *          Close MoticImageDevices, need to be invoked before exit process
 *
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
*/
long MIDP_Close();

/**
 * @brief 暂停运行,通过MIDP_Resume继续
 *        pause, invoke MIDP_Resume to continue.
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see MIDP_Resume
 *
*/
long MIDP_Pause();

/**
 * @brief 继续运行，当调用MIDP_Pause暂停后可调用该方法继续
 *        resume from pause 

 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see MIDP_Pause
 *
*/
long MIDP_Resume();

/**
 * @brief 显示或隐藏界面
 *        show/hide window
 *
 * @Param bShow:0 隐藏界面,　1,显示上次记忆的窗口，2,仅显示控制面板，3，显示视频与控制面板
 *　　　　　　　0 hide the window, 1 show window, 2 show control Panel, 3 show live image and Control Panel
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
*/
long MIDP_Show(long bShow);

/**
 * @brief 获取当前图像的格式
 *        Query image format
 *
 * @param w         图像宽度
 *                  image width
 *        h         图像高度
 *                  image height
 *        bits      色彩位深（一般是8，高位深为16）
 *                  color depth(8 bits normal image, 16bits for high depth image)
 *        channels  颜色通道（一般是RGB三个通道，单色为1个通道）
 *                  color channels(normally, we use RGB format, which has 3 channels, homochrome image only has 1 channel)
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_GetFormat(long*w, long*h, long* bits, long*channels);

/**
 * @brief 获取一帧图像数据
 *        get one frame of image
 *
 * @param buffer 缓冲区指针，必须足够容纳将取得的图像数据。
 *               The frame buffer, must be big enough for containing the image data.
 *        w      图像宽度 
 * 　　　　　　　width
 *        h      图像高度
 *               height 
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see   MIDP_GetFormat
 */
long MIDP_GetFrame(unsigned char* buffer, long w, long h);

/**
 * @brief 获取设备数
 *        Query the number of cameras 
 *
 * @return 设备数
 *          the number of cameras
 *
 */
long  MIDP_GetCameraCount();

/**
 * @brief 获取当前打开设备的索引，可通过MIDP_GetCameraName获得名称
 *        query the index of opened camera. can obtain device's name by invoke MIDP_GetCameraName 
 *
 * @return 当前打开的设备索引
 *         the index of opened camera.
 *
 * @see MIDP_GetCameraName
 */
long  MIDP_GetCurCameraIndex();

/**
 * @brief 根据索引获取设备名称
 *        query the device's name 
 * 
 * @Return 设备名称,UINCODE字符
 *         device's UINCODE name
*/
long MIDP_GetCameraName( long idx, wchar_t*name, int len );

//ASCII version
long MIDP_GetCameraNameA(long idx, char*name, int len);
/**
 * @brief 选择切换设备，切换设备后要重新获取分辨率与图像格式参数
 *        select to open camera by index, after invoke this function, all the resolutions and formats of image need to be Query again
 * @param idx 设备索引
 *            index of camera
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see MIDP_GetCameraCount 
 */
long  MIDP_SelectCamera(long idx);

/**
 * @brief 获取当前打开设备的分辨率个数
 *        get the number of resolutions
 * 
 * @return     分辨率个数
 *             the count of resolutions
 */
long  MIDP_GetResolutionCount();

/**
 * @brief 获取当前显示的分辨率索引，可通过MIDP_GetResolution获取实际分辨率
 *        get the index of current resolution, please invoke MIDP_GetResolution to query the resolution.
 *
 * @Return 当前分辨率索引
 *          the index of current resolution
 * @see MIDP_GetResolution
*/
long  MIDP_GetCurResolutionIndex();

/**
 * @brief 根据索引获取分辨率
 *        query the resolution by index
 * @param idx 分辨率索引
 *            the index of resolution
 *        w   用于获取分辨率宽度的整数指针
 *            the point to the long to receive the with of resolution
 *        h   用于获取分辨率高度的整数指针
 *            the point to the long to receive the height of resolution
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_GetResolution(long idx, long*w, long*h);

/**
 * @brief 根据分辨率大小来选择分辨率，必须为MIDP_GetResolution返回分辨率之一
 *        select the resolution by size, the w,h must be one of values query from MIDP_GetResolution
 * 
 * @param w   分辨率宽度
 *            the with of resolution
 *        h   分辨率高度
 *            the height of resolution
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see  MIDP_GetResolution
 */
long  MIDP_SelectResolution(long w, long h);

/**
 * @brief 根据分辨率索引来选择分辨率
 *        select the resolution by index
 *
 * @param idx  分辨率索引必须为0（含）到MIDP_GetResolutionCount返回值（不含)之间的值.
 *              the index of resolution must be one of integrate vaule between 0 and the value return by MIDP_GetResolutionCount
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see MIDP_GetResolutionCount
 */
long  MIDP_SelectResByIndex(long idx);

/**
 * @brief 拍照，将当前帧保存为图片，支持BMP,JPG,PNG三种格式
 *        Capture, save current image to file, support 3 format:.bmp, .jpg, .png.
 *
 * @param name, 保存图片的文件名，注意扩展名只能为".bmp", ".jpg", ".png"三种之一。对应三种图像格式。
 *　　　　　　　file name. The extension must be one of 3 format types: ".bmp", ".jpg" or ".png" 
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_Capture(wchar_t*name);

//ASCII version
long MIDP_CaptureA(char* name);
/**
 * @brief 设定自动曝光，
 *        Auto Exposure setting
 * @param bAuto 是否打开自动曝光，1打开，2关闭
 *              Activate/deactivate Auto Exposure, true activate auto exposure, false deactivate auto exposure
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_AutoExposure(long bAuto);

/**
 * @brief 查寻曝光值范围
 *        query the range of exposure value
 *
 * @param  min 最小曝光值
 *             the minimum exposure value
 *         max 最大曝光值
 *             the maximum exposure value
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_GetExposureValueRange(long* min, long* max);

/**
 * @brief 设定曝光值
 *        set exposure value
 *
 * @param  v  曝光值，必须在MIDP_GetExposureValueRange返回值范围内
 *            exposure value, must be one of value within the range return by MIDP_GetExposureValueRange
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see    MIDP_GetExposureValueRange   
 */
long MIDP_SetExposureValue(long v);

/**
 * @brief 获取曝光值
 *        get exposure value
 *
 * @param  v  返回的当前曝光值
 *            current exposure value return.
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see    MIDP_GetExposureValueRange   
 */
long MIDP_GetExposureValue(long* v);
/**
 * @brief 查寻曝光时间范围
 *        query the range of exposure time
 *
 * @param  min 最小曝光时间(10us单位)
 *             the minimum exposure time （ measurement in 10us)
 *         max 最大曝光时间(10us单位)
 *             the maximum exposure time （ measurement in 10us)
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 */
long MIDP_GetExposureTimeRange(long* min, long* max);

/**
 * @brief 设定曝光时间(10us单位)
 *        set exposure time （ measurement in 10us)
 *
 * @param  v  曝光时音（10us单位），必须在MIDP_GetExposureValueRange返回值范围内
 *            exposure time ( measurement in 10us), must be one of value within the range return by MIDP_GetExposureValueRange
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see    MIDP_GetExposureTimeRange   
 */
long MIDP_SetExposureTime(long v);

/**
 * @brief 获取曝光时间
 *        get exposure time
 *
 * @param  v  返回的当前曝光时间（单位10us)
 *            current exposure time （measurement in 10us) return.
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see    MIDP_GetExposureTimeRange   
 */
long MIDP_GetExposureTime(long* v);

/**
 * @brief 获取增益范围，如果硬件支持，这个范围就是硬件的增益，否则为软件增益
 *        Query the range of gain, this gain come from hardware if device support gain adjustment, otherwise, it is software gain
 *
 * @param  min 最小增益，
 *             minimum gain
 *         max 最大增益
 *             maximum gain
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 */
long MIDP_GetGainRange(float* min, float*max);

/**
 * @brief 设置增益，默认值为1.0
 *        set Gain, default value is 1.0
 *
 * @param  g 增益值，必须为MIDP_GetGainRange返回的范围之间的值，
 *           gain, gain value must be in the range query from MIDP_GetGainRange 
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see    MIDP_GetGainRange
 */
long MIDP_SetGain(float g);

/**
 * @brief 获取RGB增益范围，RGB为软件增益
 *        Query the range of software gain
 *
 * @param  min 最小增益，
 *             minimum gain
 *         max 最大增益
 *             maximum gain
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 */
long MIDP_GetRGBGainRange(float*min, float*max);

/**
 * @brief 设置RGB软件增益, 默认值为1.0
 *        set RGB software Gain,default value is 1.0
 *
 * @param  r 红色增益值，必须为MIDP_GetRGBGainRang返回的范围之间的值，
 *           gain of red channel, the value must be within the range query from MIDP_GetRGBGainRange
 *         g 绿色增益值，必须为MIDP_GetRGBGainRang返回的范围之间的值，    
 *           gain of green channel, the value must be within the range query from MIDP_GetRGBGainRange
 *         b 蓝色增益值，必须为MIDP_GetRGBGainRang返回的范围之间的值，
 *           gain of blue channel, the value must be within the range query from MIDP_GetRGBGainRange
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see    MIDP_GetRGBGainRange
 */
long MIDP_SetRGBGain(float r, float g, float b);

/**
 * @brief 获取亮度范围
 *        Query the range of brightness
 *
 * @param  min 最小亮度，
 *             minimum brightness
 *         max 最大亮度
 *             maximum brightness
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 */
long MIDP_GetBrightnessRange(long*min, long*max);

/**
 * @brief 设定亮度偏移, 默认值为0
 *        set offset of brightness, default value is 0
 *
 * @param  b 亮度偏移值，范围为-255~255,默认为0，
 *           offset value of brightness, range:-255~255, default value: 0
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_SetBrightness(long b);

/**
 * @brief 获取当前亮度偏移,
 *        get current offset of brightness
 *
 * @param  b 亮度偏移值
 *           offset value of brightness
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_GetBrightness(long* b);

/**
 * @brief 设定RGB各颜色的亮度偏移, 默认值为0
 *        set offset of RGB brightness, default value is 0
 *
 * @param  r 红色亮度偏移值，范围为-255~255,默认为0，
 *           offset value of Red brightness, range:-255~255, default value: 0
 *         g 绿色亮度偏移值，范围为-255~255,默认为0，
 *           offset value of Green brightness, range:-255~255, default value: 0
 *         b 蓝色亮度偏移值，范围为-255~255,默认为0，
 *           offset value of Blue brightness, range:-255~255, default value: 0
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_SetRGBBrightness(long r, long g, long b);

/**
 * @brief 设定伽马值
 *        set gamma
 *
 * @param  g 伽马值，范围为0.1~10,默认为1.0，
 *           gamma value, range: 0.1-10,default value is 1.0
 *         o 像素偏移，它为直方图增强下限值，范围为0~255,默认为0
 *           offset of pixel, it is the lower limit value of histogram enhance, range: 0~255, default: 0
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_SetGamma(float g, long o);

/**
 * @brief 获取伽马值
 *        get gamma
 *
 * @param  g 伽马值
 *           gamma value
 *         o 像素偏移，它为直方图增强下限值
 *           offset of pixel, it is the lower limit value of histogram enhanc
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_GetGamma(float *g, long *o);

/**
 * @brief 基于当前的图像计算并应用白平衡，一般情况下当前图像为背景时调用该函数。
          整个系统如果环境不变只要调用一次该函数，白平衡就会记忆在系统中，下次只要调用MIDP_ActivateWhiteBalance
 *        calculate the factors of whitebalance base on current image. normally, invoke this function once only 
 *        when current image is background image, and these factors will be record in system, you can open white balance simply invoke MIDP_ActivateWhiteBalance next time
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see MIDP_ActivateWhiteBalance
 */
long MIDP_DoWhiteBalance();

/**
 * @brief 应用白平衡，调用该函数在MIDP_DoWhiteBalance之后，一般在同样的环境下白平衡的参数不会改变,它会保留在系统中
 *　　　　下次再打开程序只要简单地调用该函数就可以应用之前记忆的白平衡
 *        Activate white balance, the whitebalance factors, which are generated after call MIDP_DoWhiteBalance last time,will remain in you system.
 *        You can simply invoke this function to activate white balance when you open the software next time. 
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see MIDP_DoWhiteBalance
 */
long MIDP_ActivateWhiteBalance(long act);

/**
 * @brief 设定白平衡参数，设定该参数后，直接调用MIDP_ActivateWhiteBalance，白平衡会依据给定的rgb来设定 *　　　　
 *        Set WhiteBalance factors, the white balance will use the input r,g,b value adjust color balance.
 *        You can invoke MIDP_ActivateWhiteBalance after setting these parameters
 *
 * @param  r,g,b 白平衡R,G,B因子，百分比数值
 *               R,G,B factors for White Balance, with percent style.
 * 
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see MIDP_ActivateWhiteBalance
 */
long MIDP_SetWBFactors(long r, long g, long b);

/**
 * @brief 获取白平衡参数，　　　　
 *        Get WhiteBalance factors
 *
 * @param  r,g,b 白平衡R,G,B因子，百分比数值
 *               R,G,B factors for White Balance, with percent style.
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 * @see MIDP_SetWBFactors
 */
long MIDP_GetWBFactors(long *r, long *g, long *b);

/**
 * @brief 设定色彩校正参数
 *        set Color Correction
 *
 * @param  val 色彩校正参数值，范围-10~10,默认值为0,正方向可以增强色彩，负值减弱色彩
 *             value of color correction, range: -10-10, default: 0, 
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_SetColorCorrection(long val);

/**
 * @brief 获取色彩校正参数
 *        get Color Correction
 *
 * @param  val 色彩校正参数值.
 *             value of color correction.
 *
 * @return 0 成功; 否则失败
 *         0 succeed, otherwise failed.
 *
 */
long MIDP_GetColorCorrection(long* val);


/**
 * @brief 获取一帧图像数据的指定区域
 *
 * @param buffer 缓冲区指针，必须足够容纳将取得的图像数据。
 *        x      图像的水平起始位置 
 *        y      图像的垂直起始位置
 *        w      图像的宽度
 *        h      图像的高度
 *        stride 缓冲区的行字节数
 *        flip   是否须翻转图像
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 * @see   MIDP_GetFormatEx
 */
long MIDP_GetFrameEx(unsigned char* buffer, long x, long y, long w, long h, long stride, bool flip);
/**
 * @undocumented
 */
double MIDP_EvaluateFocusDegree24bits(unsigned char* buffer, long width, long height);

/**
 * @brief 获取一帧24bits图像数据
 *        get one frame of image with 24bits
 *
 * @param buffer 24bits缓冲区指针，必须足够容纳将取得的图像数据。
 *               The frame buffer for 24bits color depth, must be big enough for containing the image data.
 *        w      图像宽度 
 * 　　　　　　　width
 *        h      图像高度
 *               height 
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 */
long MIDP_GetFrame24(unsigned char*buffer, long w, long h);

/**
 * @brief 设定图像的位深
 *        Set bit depth of image data.
 *
 * @param bitcount 位深，取值为8与16
 *                  Bit depth 8 or 16 bits
 *
 * @return     0 成功; 否则失败
 *             0 succeed, otherwise failed.
 *
 */
long MIDP_SetBitCount(long bitcount);