// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "lambda_scope.h"
#include "multiple_http_request.h"

#include <thread>

#define TEST_MULTIPLE_HTTP_REQUEST  (0)
#define TEST_LAMBDA_SCOPE           (0)

int _tmain(int argc, _TCHAR* argv[])
{
#if TEST_MULTIPLE_HTTP_REQUEST
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
#endif/* TEST_MULTIPLE_HTTP_REQUEST */
#if TEST_LAMBDA_SCOPE
    {
        lambda_scope test2;
        test2.start_local_ramda();
    }
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(200));
    }
#endif/* TEST_LAMBDA_SCOPE */

    printf("end");
}
