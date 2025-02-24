#pragma once

#define MAX_BUFFER_SIZE 1024

class InputBox {
    public:
	void insert(char c);
	void backspace();
	const char* get();
	void clear();

	InputBox();

    private:
	char m_Buffer[MAX_BUFFER_SIZE];
	size_t m_Cursor;
};
