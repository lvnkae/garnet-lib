/*!
 *  @file   random_generator.h
 *  @brief  [common]���������N���X
 *  @date   2017/12/18
 */
#pragma once

#include <memory>

/*!
 *  @brief  "64bit�����Ȃ�����"�͈͂̈�l����������
 *  @note   �EMT�̈�l����
 *  @note   �Eboost::random�̓r���h�ʂ�Ȃ��̂�std::random���g�p�c
 *  @note   �@VS2013�ɂ�������m�̕s���������(https://svn.boost.org/trac10/ticket/11426)
 */
class RandomGenerator
{
public:
    /*!
     *  @param  seed    ������
     *  @note   �w��Ȃ��Ȃ�n�[�h�E�F�A�������̗p
     */
     RandomGenerator(size_t seed);
     RandomGenerator();

    ~RandomGenerator();
    /*!
     *  @brief  64bit�����Ȃ������͈͂̈�l�����𓾂�
     *  @return ����
     */
    uint64_t Random();
    /*!
     *  @brief  [a,b]�̈�l�����𓾂�
     *  @param  a   ����
     *  @param  b   ���
     */
    uint64_t Random(uint64_t a, uint64_t b);

private:
    RandomGenerator(const RandomGenerator&);
    RandomGenerator& operator= (const RandomGenerator&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};
