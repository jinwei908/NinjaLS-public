#ifndef SCREENSHOTTER_H_INCLUDED
#define SCREENSHOTTER_H_INCLUDED

#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include <memory>
#include "Base64.h"

using namespace Gdiplus;
using namespace std;

namespace ScreenShotter {
    std::string savePath;
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
    {
        ///HANDLES ENCODING
        UINT num = 0;          // number of image encoders
        UINT size = 0;         // size of the image encoder array in bytes

        ImageCodecInfo* pImageCodecInfo = NULL;

        GetImageEncodersSize(&num, &size);
        if(size == 0)
        {
            return -1;  // Failure
        }

        pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
        if(pImageCodecInfo == NULL)
        {
            return -1;  // Failure
        }

        GetImageEncoders(num, size, pImageCodecInfo);

        for(UINT j = 0; j < num; ++j)
        {
            if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return j;  // Success
            }
        }

        free(pImageCodecInfo);
        return -1;  // Failure
    }

    void BitmapToJpg(HBITMAP hbmpImage, int width, int height)
    {
        ///HANDLES CONVERSION OF BITMAP TO JPG
        Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);

        CLSID pngClsid;
        int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
        if(result != -1)
            std::cout << "Encoder succeeded" << std::endl;
        else
            std::cout << "Encoder failed" << std::endl;
        std::string pathtosave = savePath + "111100001111.jpg";
        std::wstring widestr = std::wstring(pathtosave.begin(), pathtosave.end());
        p_bmp->Save(widestr.c_str(), &pngClsid, NULL);
        delete p_bmp;
    }

    std::string JpgToBase64()
    {
        ///HANDLES CONVERSION OF BITMAP TO JPG
        std::string pathtosave = savePath + "111100001111.jpg";
        FILE *f = fopen(pathtosave.c_str(), "rb");
        if (!f)         throw;
        uint32_t len = 0;
        fseek(f, 0x00, SEEK_END);
        len = ftell(f);
        fseek(f, 0x00, SEEK_SET);
        char header[1024];
        char *buffer = new char[len];
        fread(buffer, sizeof(char), len, f);
        std::string bufferStr = Base64::base64_encode_bytes(buffer, len);

        return bufferStr;
    }


    std::string ScreenCapture(int x, int y, int width, int height, char *filename)
    {
        ///HANDLES CAPTURING SCREENSHOTS HERE
        HDC hDc = CreateCompatibleDC(0);
        HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
        SelectObject(hDc, hBmp);
        BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
        BitmapToJpg(hBmp, width, height);
        DeleteObject(hBmp);
        return JpgToBase64();
    }

    std::string GetScreenshot(std::string pathToSave) {
        // Initialize GDI+.
        savePath = pathToSave;
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        int x1 = 0;
        int y1 = 0;
        int x2 = GetSystemMetrics(SM_CXSCREEN);
        int y2 = GetSystemMetrics(SM_CYSCREEN);
        std::string base64image = ScreenCapture(x1, y1, x2 - x1, y2 - y1, "111100001111.jpg");

        //Shutdown GDI+
        GdiplusShutdown(gdiplusToken);

        return base64image;
    }
}

#endif // SCREENSHOTTER_H_INCLUDED
