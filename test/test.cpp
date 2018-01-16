// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "multiple_http_request.h"

#include <thread>

int _tmain(int argc, _TCHAR* argv[])
{
#if defined(WIN32)
    setlocale( LC_ALL, "Japanese" );
#endif/* WIN32 */

    for (int32_t inx = 0; inx < 10; inx++)
    {
	    multiple_http_request test1;
        test1.muliple_request_test1();
        while (1) {
            if (test1.is_end_of_multiple_request_test1()) {
                break;
            }
        }
        test1.print_result_of_multiple_request_test1();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    printf("end");
}
