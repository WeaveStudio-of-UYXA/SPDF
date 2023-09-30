#pragma execution_character_set("utf-8") //所有含有const char的cpp文件都需要这一句（大概）
#include <VICore>
#include <VIWidgets>
#include "SPDF/SPDFPackage.h"
#include "SPDF/SPOLClassicTester.h"
#ifndef SPDF_DEVELOPER
visindigo Program {
	VIFramework Frame(sysArgv);
	// 此处放置框架的初始化参数
	// VVVVVVVVVVVVVVVVVVVV
	Frame.useDebugModeRuntime();


	// AAAAAAAAAAAAAAAAAAA
	Frame.init(); // 初始化框架
	// 此处放置您的项目的初始化代码
	// VVVVVVVVVVVVVVVVVVVV
	VIConsole::printLine(VIConsole::inSuccessStyle("Hello world!"));
	LOAD_PACKAGE(SPDF::Package);

	SPDFClassicTester* tester = new SPDFClassicTester();
	SPDF::Package::getInstance()->attachNewHostToTerminal(tester);
	SPOLStandardControllerParser* parser = new SPOLStandardControllerParser();
	parser->setControllerJSON("./SPDF/resource/SPDFDialogue.json");
	tester->Host->installParser(parser);
	tester->Host->exec("./SPDF/resource/syn.js");
	// AAAAAAAAAAAAAAAAAAA
	Frame.start(); // 启动框架，此行之后的代码将不会被执行
} ProgramEnd;
#endif