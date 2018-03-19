# -*- coding: utf-8 -*-
# for 2.7
# function関連utility
 
#   @brief  空関数を得る
#   @return 空関数
def blankFunc():
    return lambda: None

#   @brief  関数が空か調べる
#   @retval True    空
#   @note   空関数=何もしないlambdaなのでバイトコードでチェック
#   @note   もっとスマートな方法がある気がする
def isBlankFunc(func):
    return func.func_code.co_code == 'd\x00\x00S'
