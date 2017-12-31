/*!
 *  @file   securities_session_sbi_util.hpp
 *  @brief  SBI�،��T�C�g�Ƃ̃Z�b�V�����Ǘ�(utility)
 *  @date   2017/12/29
 *  @note   securities_session_sbi�ł����g��Ȃ�static�Ȓ萔��֐��Q
 *  @note   �����璼��include����B�\�[�X���������������B
 */


//! URL:SBI(PC):���C���Q�[�g
const wchar_t URL_MAIN_SBI_MAIN[] = L"https://site1.sbisec.co.jp/ETGate/";
//! URL:SBI(PC):�|�[�g�t�H���I�\��
const wchar_t URL_MAIN_SBI_PORTFOLIO[] = L"?portforio_id=2&indicate_id=5&sep_ind_specify_kbn=0&sort_kbn=0&sort_id=01&pts_kbn=0&_ControlID=WPLETpfR001Control&_PageID=WPLETpfR001Rlst10&_DataStoreID=DSWPLETpfR001Control&_ActionID=reloadPfList";
//! URL:SBI(PC):�|�[�g�t�H���I�]���g�b�v(site0�ւ̃��O�C�������˂�)
const wchar_t URL_MAIN_SBI_TRANS_PF_LOGIN[] = L"?_ControlID=WPLETsmR001Control&_DataStoreID=DSWPLETsmR001Control&_PageID=WPLETsmR001Sdtl12&_ActionID=NoActionID&sw_page=WNS001&sw_param1=portfolio&sw_param2=pfcopy&cat1=home&cat2=none";
//! URL:SBI(PC):�|�[�g�t�H���I�]���m�F
const wchar_t URL_MAIN_SBI_TRANS_PF_CHECK[] = L"https://site0.sbisec.co.jp/marble/portfolio/pfcopy/selectcheck.do";
//! URL:SBI(PC):�|�[�g�t�H���I�]�����s
const wchar_t URL_MAIN_SBI_TRANS_PF_EXEC[] = L"https://site0.sbisec.co.jp/marble/portfolio/pfcopy/transmission.do";
//! URL:SBI(mobile):���O�C��
const wchar_t URL_BK_LOGIN[] = L"https://k.sbisec.co.jp/bsite/visitor/loginUserCheck.do";
//! URL:SBI(mobile):��{�`
const wchar_t URL_BK_BASE[] = L"https://k.sbisec.co.jp/bsite/member/";
//! URL:SBI(mobile):�g�b�v�y�[�W
const wchar_t URL_BK_TOP[] = L"menu.do";
//! URL:SBI(mobile):�|�[�g�t�H���I�o�^�m�F
const wchar_t URL_BK_STOCKENTRYCONFIRM[] = L"portfolio/lumpStockEntryConfirm.do";
//! URL:SBI(mobile):�|�[�g�t�H���I�o�^���s
const wchar_t URL_BK_STOCKENTRYEXECUTE[] = L"portfolio/lumpStockEntryExecute.do";
//! URL:SBI(mobile):��������(��)
const wchar_t URL_BK_BUYORDER_ENTRY[] = L"buyOrderEntry.do";
//! URL:SBI(mobile):�����m�F(��)
const wchar_t URL_BK_BUYORDER_CONFIRM[] = L"buyOrderEntryConfirm.do";
//! URL:SBI(mobile):�������s(��)
const wchar_t URL_BK_BUYORDER_EXECUTE[] = L"buyOrderEx.do";
//! URL:SBI(mobile):��������(��)
const wchar_t URL_BK_SELLORDER_ENTRY[] = L"sellOrderEntry.do";
//! URL:SBI(mobile):�����m�F(��)
const wchar_t URL_BK_SELLORDER_CONFIRM[] = L"sellOrderEntryConfirm.do";
//! URL:SBI(mobile):�������s(��)
const wchar_t URL_BK_SELLORDER_EXECUTE[] = L"sellOrderEx.do";

//! �p�����[�^���F����
const wchar_t PARAM_NAME_SPOT[] = L"stock/";
//! �p�����[�^���F�M�p
const wchar_t PARAM_NAME_LEVERAGE[] = L"margin/";
//! �p�����[�^���F�����������R�[�h
const wchar_t PARAM_NAME_ORDER_STOCK_CODE[] = L"ipm_product_code";
//! �p�����[�^���F��������������
const wchar_t PARAM_NAME_ORDER_INVESTIMENTS[] = L"market";
//! �p�����[�^���F�����敪
const wchar_t PARAM_NAME_ORDER_TYPE[] = L"cayen.buysellKbn";

/*!
 *  @brief  �����X�e�b�v
 */
enum eOrderStep {
    OSTEP_INPUT,    // ����
    OSTEP_CONFIRM,  // �m�F
    OSTEP_EXECUTE,  // ���s

    NUM_OSTEP,
};
const wchar_t* URL_BK_ORDER[NUM_ORDER][NUM_OSTEP] = {
    { nullptr,                  nullptr,                    nullptr                     },
    { URL_BK_BUYORDER_ENTRY,    URL_BK_BUYORDER_CONFIRM,    URL_BK_BUYORDER_EXECUTE     },
    { URL_BK_SELLORDER_ENTRY,   URL_BK_SELLORDER_CONFIRM,   URL_BK_SELLORDER_EXECUTE    },
    { nullptr,                  nullptr,                    nullptr                     },
    { nullptr,                  nullptr,                    nullptr                     },
};

/*!
 *  @brief  �������ʂ�"SBI�p�����o�^�p������R�[�h"�ɕϊ�
 *  @param  investments_type    ��������
 */
std::wstring GetSbiInvestimentsCode(eStockInvestmentsType investments_type)
{
    switch (investments_type)
    {
    case INVESTMENTS_TOKYO:
        return L"TKY";
    case INVESTMENTS_NAGOYA:
        return L"NGY";
    case INVESTMENTS_FUKUOKA:
        return L"FKO";
    case INVESTMENTS_SAPPORO:
        return L"SPR";
    case INVESTMENTS_PTS:
        return L"JNX";
    default:
        return L"";
    }
}

/*!
 *  @brief  "SBI�p�����o�^�p������R�[�h"���������ʂ��ɕϊ�
 *  @param  code    SBI�p�����o�^�p������R�[�h
 */
eStockInvestmentsType GetStockInvestmentsTypeFromSbiCode(const std::string& code)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
    std::wstring codeT = utfconv.from_bytes(code.c_str());

    if (GetSbiInvestimentsCode(INVESTMENTS_TOKYO).compare(codeT) == 0) {
        return INVESTMENTS_TOKYO;
    }
    if (GetSbiInvestimentsCode(INVESTMENTS_PTS).compare(codeT) == 0) {
        return INVESTMENTS_PTS;
    }
    if (GetSbiInvestimentsCode(INVESTMENTS_NAGOYA).compare(codeT) == 0) {
        return INVESTMENTS_NAGOYA;
    }
    if (GetSbiInvestimentsCode(INVESTMENTS_FUKUOKA).compare(codeT) == 0) {
        return INVESTMENTS_FUKUOKA;
    }
    if (GetSbiInvestimentsCode(INVESTMENTS_SAPPORO).compare(codeT) == 0) {
        return INVESTMENTS_SAPPORO;
    }
    return INVESTMENTS_NONE;
}

/*!
 *  @brief  ���������pURL�\�z
 *  @param[in]  b_leverage  �M�p�t���O
 *  @param[in]  sub_url     �ړIURL
 *  @param[out] url         �i�[��
 */
void BuildOrderURL(bool b_leverage, const std::wstring& sub_url, std::wstring& url)
{
    url = URL_BK_BASE;
    if (b_leverage) {
        url += PARAM_NAME_LEVERAGE + sub_url; // �M�p
    } else {
        url += PARAM_NAME_SPOT + sub_url; // ����
    }
}

/*!
 *  @brief  �_�~�[�̃|�[�g�t�H���I������(form data�p)�\�z
 *  @param[in]  portfolio_number    �o�^��|�[�g�t�H���I�ԍ�
 *  @param[in]  max_code_entry      �ő�����o�^��
 *  @param[out] dst                 �i�[��
 *  @note   ��SBI(backup)�̓o�^�m�F�ɓ����邽�߂̂��̂Ȃ̂Œ��g�̓_�~�[�ŗǂ�
 *  @note   ���\��SBU(backup)�őΏۂƂȂ�|�[�g�t�H���I���쐬(�g�m��)���Ă�������
 */
void BuildDummyPortfolioForFormData(const int32_t portfolio_number, const size_t max_code_entry, std::wstring& dst)
{
    // 'page'��'total_count'�̓_�~�[�Ɍ��炸'0'�Œ�
    utility::AddFormDataParamToString(L"page", L"0", dst);
    utility::AddFormDataParamToString(L"list_number", std::to_wstring(portfolio_number), dst);
    utility::AddFormDataParamToString(L"total_count", L"0", dst);
    // �o�^�����͋�ŗǂ����^�O�����͕K�v
    const std::wstring NAME_NUMBER(L"npm");
    const std::wstring TAG_STOCK_CODE(L"pcode_");
    const std::wstring TAG_INVESTIMENTS_CODE(L"mcode_");
    for (size_t inx = 0; inx < max_code_entry; inx++) {
        std::wstring index_str(std::to_wstring(inx));
        utility::AddFormDataParamToString(NAME_NUMBER, index_str, dst);
        utility::AddFormDataParamToString(TAG_STOCK_CODE+index_str, L"", dst);
        utility::AddFormDataParamToString(TAG_INVESTIMENTS_CODE+index_str, L"", dst);
    }
    utility::AddFormDataParamToString(L"submit_update", L"�o�^�E�ҏW�m�F", dst);
}

/*!
 *  @brief  �{�o�^�p�|�[�g�t�H���I������(form data�p)�\�z
 *  @param[in]  portfolio_number    �o�^��|�[�g�t�H���I�ԍ�
 *  @param[in]  max_code_entry      �ő�����o�^��
 *  @param[in]  monitoring_code     �o�^(�Ď�)����
 *  @param[in]  investments_type    ����������(�S�����ŋ���)
 *  @param[in]  regist_id           �o�^�p���j�[�NID(�o�^�m�F�������瓾��)
 *  @param[out] dst                 �i�[��
 */
void BuildPortfolioForFormData(const int32_t portfolio_number, 
                               const size_t max_code_entry,
                               const std::vector<uint32_t>& monitoring_code,
                               eStockInvestmentsType investments_type,
                               int64_t regist_id,
                               std::wstring& dst)
{
    utility::AddFormDataParamToString(L"list_number", std::to_wstring(portfolio_number), dst);
    //
    const std::wstring TAG_NUMBER(L"list_detail_number");
    const std::wstring TAG_STOCK_CODE(L"product_code");
    const std::wstring TAG_INVESTIMENTS_CODE(L"se_investments_code");
    const std::wstring investments_code(GetSbiInvestimentsCode(investments_type));
    size_t entry = monitoring_code.size();
    if (max_code_entry < entry) {
        entry = max_code_entry;
    }
    for (size_t inx = 0; inx < entry; inx++) {
        utility::AddFormDataParamToString(TAG_NUMBER, std::to_wstring(inx), dst);
        utility::AddFormDataParamToString(TAG_STOCK_CODE, std::to_wstring(monitoring_code[inx]), dst);
        utility::AddFormDataParamToString(TAG_INVESTIMENTS_CODE, investments_code, dst);
    }
    for (size_t inx = entry; inx < max_code_entry; inx++) {
        utility::AddFormDataParamToString(TAG_NUMBER, std::to_wstring(inx), dst);
        utility::AddFormDataParamToString(TAG_STOCK_CODE, L"", dst);
        utility::AddFormDataParamToString(TAG_INVESTIMENTS_CODE, L"", dst);
    }
    utility::AddFormDataParamToString(L"submit", L"�o�^", dst);
    utility::AddFormDataParamToString(L"regist_id", std::to_wstring(regist_id), dst);
}

/*!
 *  @brief  ���������p������(form data�p)�\�z
 *  @param[in]  order       ��������
 *  @param[in]  pass
 *  @param[in]  regist_id   �o�^�p���j�[�NID(�O�������瓾��)
 *  @param[out] dst         �i�[��
 */
void BuildOrderForFormData(const StockOrder& order, const std::wstring& pass, int64_t regist_id, std::wstring& dst)
{
    const utility::sFormDataParam ORDER_FORM[] = {
        { L"cayen.isStopOrder",         L"false"},  // �t�����t���O
        { L"caLiKbn" ,                  L"today"},  // ����
        { L"limit",                     L""     },  // ���Ԏw��(�������ȊO��YYYYMMDD�Ŏw��)
        { L"hitokutei_trade_kbn",       L"-"    },  // �a����敪(���) ��������Ȃ�ς����ۂ�
        { L"cayen.sor_select",          L"false"},  // SOR�s�v
    };
    for (uint32_t inx = 0; inx < sizeof(ORDER_FORM)/sizeof(utility::sFormDataParam); inx++) {
        utility::AddFormDataParamToString(ORDER_FORM[inx], dst);
    }
    utility::AddFormDataParamToString(L"regist_id", std::to_wstring(regist_id), dst);
    utility::AddFormDataParamToString(L"brand_cd", std::to_wstring(order.m_code.GetCode()), dst);
    utility::AddFormDataParamToString(PARAM_NAME_ORDER_STOCK_CODE, std::to_wstring(order.m_code.GetCode()), dst);
    utility::AddFormDataParamToString(L"market", GetSbiInvestimentsCode(order.m_investiments), dst);
    utility::AddFormDataParamToString(L"quantity", std::to_wstring(order.m_number), dst);
    utility::AddFormDataParamToString(L"price", utility::ToWstringOrder(order.m_value, 1), dst); // �����_��1�ʂ܂ō̗p
    utility::AddFormDataParamToString(L"password", pass, dst);
    if (order.m_b_leverage) {
        utility::AddFormDataParamToString(L"payment_limit", L"6", dst);   // >ToDo< ��ʐM�p/���v��̑Ή�
    }
    switch (order.m_type)
    {
    case ORDER_BUY:
        utility::AddFormDataParamToString(PARAM_NAME_ORDER_TYPE, L"buy", dst);
        break;
    case ORDER_SELL:
        utility::AddFormDataParamToString(PARAM_NAME_ORDER_TYPE, L"sell", dst);
        break;
    };
}
