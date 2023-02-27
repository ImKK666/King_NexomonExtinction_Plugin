#pragma once
#include <Windows.h>
#include <string>

typedef unsigned long error_status_t;

extern handle_t nvdaControllerBindingHandle;
extern RPC_IF_HANDLE nvdaController_NvdaController_v1_0_c_ifspec;
extern RPC_IF_HANDLE NvdaController_v1_0_c_ifspec;
extern RPC_IF_HANDLE nvdaController_NvdaController_v1_0_s_ifspec;

class nvdaController {
	HMODULE nvda;

	typedef error_status_t( __stdcall* nvdaController_testIfRunning_t )( void );
	typedef error_status_t( __stdcall* nvdaController_speakText_t )( const wchar_t* );
	typedef error_status_t( __stdcall* nvdaController_cancelSpeech_t )( void );
	typedef error_status_t( __stdcall* nvdaController_brailleMessage_t )( const wchar_t* );

	nvdaController_testIfRunning_t pfn_nvdaController_testIfRunning;
	nvdaController_speakText_t pfn_nvdaController_speakText;
	nvdaController_cancelSpeech_t pfn_nvdaController_cancelSpeech;
	nvdaController_brailleMessage_t pfn_nvdaController_brailleMessage;

public:
	nvdaController( ) : nvda( nullptr ),
						pfn_nvdaController_testIfRunning( nullptr ),
						pfn_nvdaController_speakText( nullptr ),
						pfn_nvdaController_cancelSpeech( nullptr ),
						pfn_nvdaController_brailleMessage( nullptr ) {
	}

	bool init( std::string lib ) {
		nvda = LoadLibraryA( lib.c_str( ) );
		if ( nvda ) {
			pfn_nvdaController_testIfRunning = ( nvdaController_testIfRunning_t ) GetProcAddress( nvda, "nvdaController_testIfRunning" );
			pfn_nvdaController_speakText = ( nvdaController_speakText_t ) GetProcAddress( nvda, "nvdaController_speakText" );
			pfn_nvdaController_cancelSpeech = ( nvdaController_cancelSpeech_t ) GetProcAddress( nvda, "nvdaController_cancelSpeech" );
			pfn_nvdaController_brailleMessage = ( nvdaController_brailleMessage_t ) GetProcAddress( nvda, "nvdaController_brailleMessage" );

			if ( pfn_nvdaController_testIfRunning && pfn_nvdaController_speakText && pfn_nvdaController_cancelSpeech && pfn_nvdaController_brailleMessage ) {
				return true;
			}
		}

		return false;
	}

	error_status_t nvdaController_testIfRunning( ) {
		return pfn_nvdaController_testIfRunning( );
	}

	error_status_t nvdaController_speakText( const wchar_t* text ) {
		return pfn_nvdaController_speakText( text );
	}

	error_status_t nvdaController_cancelSpeech( ) {
		return pfn_nvdaController_cancelSpeech( );
	}

	error_status_t nvdaController_brailleMessage( const wchar_t* message ) {
		return pfn_nvdaController_brailleMessage( message );
	}
};
extern nvdaController* g_nvda;