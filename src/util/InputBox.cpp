#include "InputBox.hpp"

void InputBox::insert(char c)
{
    if (m_Cursor < MAX_BUFFER_SIZE - 1) m_Buffer[m_Cursor++] = c;
}

void InputBox::backspace()
{
    if (m_Cursor > 0) m_Cursor--;
}

const char* InputBox::get()
{
    m_Buffer[m_Cursor] = '\0';
    return m_Buffer;
}

void InputBox::clear()
{
    m_Cursor = 0;
    m_Buffer[m_Cursor] = '\0';
}

InputBox::InputBox() :
    m_Buffer{0},
    m_Cursor(0)
{

}
