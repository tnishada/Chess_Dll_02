#ifdef CHESS_DLL_2_EXPORTS
#define CHESS_DLL_API __declspec(dllexport) 
#else
#define CHESS_DLL_API __declspec(dllimport) 
#endif


