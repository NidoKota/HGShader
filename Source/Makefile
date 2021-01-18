compileRun:
	g++ Main.cpp ShaderFuncs.cpp EventHandlers/ShaderDisplayer.cpp EventHandlers/InputManager.cpp EventHandlers/FPSCounter.cpp EventHandlers/DoubleLayerDisplayer.cpp Library/ShaderUtility.cpp Library/Event.cpp -o Sample -lhg -std=c++11 -pthread
	open -a HGDisplayer
	./Sample

clean:
	$(RM) Sample