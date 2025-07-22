#pragma once

#ifdef _CORE_DLL
	#define CoreDLL_EXP __declspec(dllexport)
#else
	#define CoreDLL_EXP __declspec(dllimport)
#endif

#ifdef _DATABASE_DLL
	#define DatabaseDLL_EXP __declspec(dllexport)
#else
	#define DatabaseDLL_EXP __declspec(dllimport)
#endif

#ifdef _APPLICATION_DLL
	#define ApplicationDLL_EXP __declspec(dllexport)
#else
	#define ApplicationDLL_EXP __declspec(dllimport)
#endif

#ifdef _DOMAIN_DLL
	#define DomainDLL_EXP __declspec(dllexport)
#else
	#define DomainDLL_EXP __declspec(dllimport)
#endif

#ifdef _UIDIALOG_DLL
	#define UIDialogDLL_EXP __declspec(dllexport)
#else
	#define UIDialogDLL_EXP __declspec(dllimport)
#endif

#ifdef _UIVIEW_DLL
	#define UIViewDLL_EXP __declspec(dllexport)
#else
	#define UIViewDLL_EXP __declspec(dllimport)
#endif

#ifdef _DOCUMENT_DLL
	#define DocumentDLL_EXP __declspec(dllexport)
#else
	#define DocumentDLL_EXP __declspec(dllimport)
#endif