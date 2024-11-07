//#include "VisualText.h"
#include "VisualText.h"

void ProjectInitializer::init_SDL_image()
{
    using namespace vtcore::logsys;
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP))
    {
        vtcore::lst.logIn("Image loader init additional format error", LOG_PRIORITY_WARN, LOG_CATEGORY_APPLICATION);
        additional_image_support_ = false;
        if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP))
        {
            vtcore::lst.logIn("Image loader init WEBP format error", LOG_PRIORITY_WARN, LOG_CATEGORY_APPLICATION);
            webp_support_ = false;
            if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
            {
                vtcore::lst.logIn("Image loader init error", LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
                throw vtcore::SDL_init_error();
            }
            else
                vtcore::lst.logIn("Image loader init incomplete initialization", LOG_PRIORITY_WARN, LOG_CATEGORY_APPLICATION);
        }
        else
            vtcore::lst.logIn("Image loader init incomplete initialization", LOG_PRIORITY_WARN, LOG_CATEGORY_APPLICATION);
    }
    else
        vtcore::lst.logIn("Image loader init initialization successful", LOG_PRIORITY_WARN, LOG_CATEGORY_APPLICATION);
}

int main()
{
    using namespace std;
    ProjectInitializer PINIT;
    vtcore::lst.setAllPriority(vtcore::logsys::LOG_PRIORITY_INVALID);
    vtasset::ThemeTemplateGenerator TTG;
    TTG.newGenerator(R"(E:\TestFile)", R"(E:\VtAsset.vtbp)");
    vtasset::BinaryPack bpack(R"(E:\VtAsset.vtbp)");
    vttexture::Font font1(bpack, 0, 32);
    vtcore::Window window("NULL", 1960, 1080);
    SDL_Surface* surface = font1.getTextSurface_Blended("A fox jump to a lazy dog!!", { 0, 0, 0, 255 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window.getRendererHinding(), surface);
    if (texture == nullptr)
    {
        cout << SDL_GetError() << endl;
    }
    bool quit = false;
    vtcore::RatioRect rect(window, {0.5, 0.5, 0.1, 0.1});
    SDL_FRect r = rect(texture->w, texture->h);
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(window.getRendererHinding(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(window.getRendererHinding());

        SDL_RenderTexture(window.getRendererHinding(), texture, nullptr, &(r));

        window.presentRender();
    }
}


//int main(int, char**)
//{
//    using namespace vtcore;
//    // Setup SDL
//    ProjectInitializer initializer;
//    vtcore::lst.setAllPriority(logsys::LOG_PRIORITY_INVALID);
//    vtcore::lst.logIn("Start!", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_INPUT);
//    // Create window with SDL_Renderer graphics context
//    Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
//    Window win1("Example window", 1280, 720, window_flags);
//    win1.setRenderVSync(1);
//    SDL_LogMessage(SDL_LOG_CATEGORY_ASSERT, SDL_LOG_PRIORITY_CRITICAL, "HelloWorld");
//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    //ImGui::StyleColorsDark();
//    ImGui::StyleColorsLight();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplSDL3_InitForSDLRenderer(win1.getWindowHinding(), win1.getRendererHinding());
//    ImGui_ImplSDLRenderer3_Init(win1.getRendererHinding());
//
//    // Load Fonts
//    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
//    // - Read 'docs/FONTS.md' for more instructions and details.
//    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
//    //io.Fonts->AddFontDefault();
//    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
//    //IM_ASSERT(font != nullptr);
//
//    // Our state
//    bool show_demo_window = true;
//    bool show_another_window = false;
//    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//    // Main loop
//    bool done = false;
//#ifdef __EMSCRIPTEN__
//    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
//    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
//    io.IniFilename = nullptr;
//    EMSCRIPTEN_MAINLOOP_BEGIN
//#else
//    while (!done)
//#endif
//    {
//        // Poll and handle events (inputs, window resize, etc.)
//        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
//        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
//        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
//        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
//        SDL_Event event;
//        while (SDL_PollEvent(&event))
//        {
//            ImGui_ImplSDL3_ProcessEvent(&event);
//            if (event.type == SDL_EVENT_QUIT)
//                done = true;
//            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(win1.getWindowHinding()))
//                done = true;
//        }
//        if (win1.isMinimized())
//        {
//            SDL_Delay(10);
//            continue;
//        }
//
//        // Start the Dear ImGui frame
//        ImGui_ImplSDLRenderer3_NewFrame();
//        ImGui_ImplSDL3_NewFrame();
//        ImGui::NewFrame();
//
//        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//        if (show_demo_window)
//            ImGui::ShowDemoWindow(&show_demo_window);
//
//        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//        {
//            static float f = 0.0f;
//            static int counter = 0;
//
//            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);
//
//            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//                counter++;
//            ImGui::SameLine();
//            ImGui::Text("counter = %d", counter);
//
//            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//            ImGui::End();
//        }
//
//        // 3. Show another simple window.
//        if (show_another_window)
//        {
//            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//            ImGui::Text("Hello from another window!");
//            if (ImGui::Button("Close Me"))
//                show_another_window = false;
//            ImGui::End();
//        }
//
//        // Rendering
//        ImGui::Render();
//        //SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
//        SDL_SetRenderDrawColorFloat(win1.getRendererHinding(), clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//        win1.renderClear();
//        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), win1.getRendererHinding());
//        win1.presentRender();
//    }
//#ifdef __EMSCRIPTEN__
//    EMSCRIPTEN_MAINLOOP_END;
//#endif
//
//    // Cleanup
//    ImGui_ImplSDLRenderer3_Shutdown();
//    ImGui_ImplSDL3_Shutdown();
//    ImGui::DestroyContext();
//
//    return 0;
//}