#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>

static bool b_exit = false;

void thread()
{
	while ( !b_exit )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

		static bool b_hidden = false;
		static HWND _hwnd;

		//show hidden window
		if ( GetAsyncKeyState( 0x06 ) )
		{
			if ( !b_hidden )
				continue;

			::ShowWindow( _hwnd, SW_SHOW );
			b_hidden = false;
		}

		//hide topmost window
		else if ( GetAsyncKeyState( 0x05 ) )
		{
			if ( b_hidden )
				continue;

			_hwnd = GetForegroundWindow();
			if ( !_hwnd )
				continue;

			::ShowWindow( _hwnd, SW_HIDE );
			b_hidden = true;
		}

		//exit process
		else if ( GetAsyncKeyState( VK_F11 ) )
			b_exit = true;
	}
}

//on program end
void eexit()
{
	MessageBoxA( GetConsoleWindow(), "Exiting", "Exiting", 0 );
}

//on click-X or ctrl+ c
BOOL WINAPI OnConsoleClose( DWORD dwCtrlType )
{
	b_exit = true; // set a flag that the console wants us to close
	ExitThread( 0 ); // kill this thread immediately so it doesn't make the console stuck
	return FALSE;
}

int main()
{
	std::atexit( eexit );
	std::at_quick_exit( eexit );
	SetConsoleCtrlHandler( OnConsoleClose, TRUE ); // handle close requests from the console

	FreeConsole();

	std::thread t1( thread );

	while ( !b_exit )
		std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );

	t1.join();
	return 0x01;
}
