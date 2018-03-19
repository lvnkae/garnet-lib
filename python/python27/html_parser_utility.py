# -*- coding: utf-8 -*-
# for 2.7
# htmlパース関連utility
 
import urllib2
from HTMLParser import HTMLParser

#   @brief  文字列中の','を全部削除する
#   @param  src 入力文字列
#   @return 削除後の文字列
def deleteComma(src):
    while (',' in src):
        src = src.replace(',','')
    return src

#   @brief  文字列中の' '(半角スペース)を全部削除する
#   @param  src 入力文字列
#   @return 削除後の文字列
def deleteSpace(src):
    while (' ' in src):
        src = src.replace(' ','')
    return src

#   @brief  文字列中の'&nbsp;'を全て' '(半角スペース)に置き換える
#   @param  src 入力文字列
#   @return 削除後の文字列
def deleteNBSP(src):
    while ('&nbsp' in src):
        src = src.replace('&nbsp',' ')
    return src
    
#   @brief  文字列をスペース・改行で分割
#   @param  src 入力文字列
#   @return 分割後文字列群(list)
def splitBySPCRLF(src):
    ret_str = []
    div_lf = src.split('\n');
    for str_div_lf in div_lf:
        if str_div_lf:
            div_sp = str_div_lf.split(' ')
            for str_div_sp in div_sp:
                if str_div_sp:
                    if not str_div_sp == '\r':
                        ret_str.append(str_div_sp)
    return ret_str

#   @brief  listからkeyを探してindexを返す
#   @param  src 対象list
#   @param  key 探すkey
#   @retval -1  見つからず
def searchTdDataTag(src, key):
    key_inx = 0
    for tag in src:
        if tag == key:
            return key_inx
        key_inx += 1
    return -1

#   @brief  二次元listからkeyを探してindexを返す
#   @param  src 対象list
#   @param  inx 参照するlist要素のindex
#   @param  key 探すkey
def searchTdDataTag_index(src, inx, key):
    key_inx = 0
    for elem in src:
        if elem[inx] == key:
            return key_inx
        key_inx += 1
    return -1

#   @brief  [Debug]Shift-JISで送られてきたhtmlをUTF8に変換してからファイル出力する
#   @param  html_sjis   response(html/Shift-JIS)
def debugOutputShiftJisHTMLToFile(html_sjis, filename):

    f = open(filename, 'w')
    f.write(html_sjis.decode('cp932').encode('utf-8'))
    f.close()

#   @brief  [Debug]UTF8で送られてきたhtmlをそのままファイル出力する
#   @param  html_u8     response(html/UTF-8)
def debugOutputHTMLToFile(html_u8, filename):

    f = open(filename, 'w')
    f.write(html_u8)
    f.close()
