#include <Windows.h>
#include <string>
#include <iostream>
#include <thread>

#include <nvdaController.h>
#include "VersionHijack.hpp"

#include "King_il2cpp/King_il2cpp.hpp"


//public unsafe void ShowText(string text, string side = "left", string name = "", string type = null)
typedef void( __fastcall *Dialogue_ShowText_t )( IL2CPP::Object *, System::String* , System::String* , System::String* , System::String*  );
Dialogue_ShowText_t Dialogue_ShowText;

void __fastcall MyDialogue_ShowText( IL2CPP::Object *this_, System::String* text, System::String* side, System::String* name, System::String* type ) {
    //printf( "text:%llX\n", text );

    printf( "text:%S\n", text->to_wstring( ).c_str( ) );
    printf( "side:%S\n", side->to_wstring( ).c_str( ) );
    printf( "name:%S\n", name->to_wstring( ).c_str( ) );
    printf( "type:%S\n\n", type->to_wstring( ).c_str( ) );

    nvdaController_speakText( ( /*name->to_wstring( ) + L":" + */text->to_wstring( ) ).c_str( ) );

    Dialogue_ShowText( this_, text, side, name, type );
}

DWORD WINAPI InitThread( ) {
    // Console
    {
        AllocConsole( );
        FILE *file;
        freopen_s( &file, "CONIN$", "r", stdin );
        freopen_s( &file, "CONOUT$", "w", stdout );
        SetConsoleTitleA( "[King] Debug Console" );
        setlocale( LC_CTYPE, setlocale( LC_ALL, "" ) );
    }


    error_status_t status = nvdaController_testIfRunning( );
    if( status != 0 ) {
        printf( "未初始化的 NVDA 通讯接口!!\n" );
        printf( "未初始化的 NVDA 通讯接口!!" );
    }

    IL2CPP::Initialize( );
    IL2CPP::Attach( );

    auto assembly_csharp           = IL2CPP::Assembly::Resolve( "Assembly-CSharp" );
    auto assembly_csharp_namespace = assembly_csharp->GetNamespace( "" );
    auto class_Dialogue            = assembly_csharp_namespace->GetClass( "Dialogue" );
    printf( "assembly_csharp:0x%p\nassembly_csharp_namespace:0x%p\nclass_Dialogue:0x%p\n", assembly_csharp, assembly_csharp_namespace, class_Dialogue );

	auto Method_Dialogue_ShowText = class_Dialogue->GetMethod( "ShowText", 4 )->FindFunction< Dialogue_ShowText_t >( );
    printf( "Method_Dialogue_ShowText:%p\n", Method_Dialogue_ShowText );

    Dialogue_ShowText = assembly_csharp_namespace->GetClass( "Dialogue" )->GetMethod( "ShowText", 4 )->Hook< Dialogue_ShowText_t >( MyDialogue_ShowText );

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
    if( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
        DllHijack::Initialize( );

        const auto v_str_cmdline = std::wstring( GetCommandLineW( ) );
        if( v_str_cmdline.find( L"Nexomon Extinction.exe" ) != std::wstring::npos ) {
            std::thread( InitThread ).detach( );
        }
    }

    return TRUE;
}