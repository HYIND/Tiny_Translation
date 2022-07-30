#include <windows.h>
#include <windowsx.h>
#include "Translator.h"
#include "time.h"

HHOOK mouseHook=NULL;
int MoveX, MoveY;
clock_t down_time=0,up_time=0;
bool Delimit_toggle=false;
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    if(Delimit_toggle)
    {
        if(nCode == HC_ACTION) //当nCode等于HC_ACTION时，要求得到处理
        {
            //       if(wParam==WM_MOUSEWHEEL//滑轮
            //        ||wParam==WM_RBUTTONDOWN//鼠标右键按下
            //        ||wParam==WM_LBUTTONDOWN//鼠标左键按下
            //        ||wParam==WM_RBUTTONUP//鼠标右键弹起的消息
            //        ||wParam==WM_LBUTTONUP//鼠标左键弹起的消息
            //        ||wParam==WM_MOUSEMOVE)//鼠标的移动

            //            if(wParam==WM_MOUSEMOVE)
            //            {
            //                            PMSLLHOOKSTRUCT mouse = (PMSLLHOOKSTRUCT)lParam;
            //                            qDebug()<<mouse->pt.x<<mouse->pt.y;
            //                            MoveX=mouse->pt.x;
            //                            MoveY=mouse->pt.y;
            //            }
            if(wParam==WM_LBUTTONDOWN)//左键按下
            {
                down_time=clock();
            }

            if(wParam==WM_LBUTTONUP)//左键弹起
            {
                up_time=clock();
                if(up_time-down_time>100)
                {
                    PMSLLHOOKSTRUCT mouse = (PMSLLHOOKSTRUCT)lParam;
                    MoveX=mouse->pt.x,
                    MoveY=mouse->pt.y;
                    tran.Tranlate_back_emit();
                }
            }
        }
    }
    return CallNextHookEx(mouseHook,nCode,wParam,lParam);//否则，如果返回给下一个钩子子程处理
}
void SetMouseHook()
{
    mouseHook =SetWindowsHookEx(WH_MOUSE_LL,mouseProc,nullptr,0);//注册鼠标钩子
}
