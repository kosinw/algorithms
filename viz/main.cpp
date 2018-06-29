#include <Windows.h>
#include <iostream>
#include <map>
#include <stdint.h>

#include <pathfind.hh>
#include <windraw/windraw.hpp>

// flags for the grid
enum : int32_t
{
    NONE    = 0,
    START   = 1 << 0,
    END     = 1 << 1,
    VISITED = 1 << 2,
    BLOCKED = 1 << 3
};

// grid settings
std::map<size_t, int32_t> g_cells; // index, property
size_t g_gridWidth;
size_t g_gridHeight;

// tracking of previous start and end indices on the graph
int g_lastStartIndex = -1;
int g_lastEndIndex   = -1;

// tracking of left and mouse button states
bool g_leftMouseDown  = false;
bool g_rightMouseDown = false;

// tracking of the state of the enter button
bool g_enterButtonDown = false;

// dimensions of the window
float g_windowWidth;
float g_windowHeight;

// pathfinder and if path is draw flag
A::Pathfinder g_pathfinder;
bool g_pathDrawn = false;

void resizeGrid(size_t gridWidth, size_t gridHeight);
void update(wd::Window& window);
void render(const wd::Window& window);

template <class T>
const T& clamp(const T& x, const T& upper, const T& lower)
{
    return min(upper, max(x, lower));
}

// override ColorI operator+
wd::ColorI operator+(const wd::ColorI& a, const wd::ColorI& b)
{
    wd::ColorI c;

    c.r = clamp(a.r + b.r, 255, 0);
    c.g = clamp(a.g + b.g, 255, 0);
    c.b = clamp(a.b + b.b, 255, 0);

    return c;
}

// override ColorI operator+=
wd::ColorI operator+=(wd::ColorI& a, const wd::ColorI& b)
{
    a = a + b;
    return a;
}

int main(int, char**)
{
    wd::Window window(wd::Spec(720, 720), L"A* Visualization by Kosi Nwabueze", wd::Style::Close | wd::Style::Titlebar);

    wd::Size2 dimensions;
    window.getDimensions(dimensions);

    g_windowWidth  = dimensions.width;
    g_windowHeight = dimensions.height;

    resizeGrid(20, 20);

    LARGE_INTEGER li = { 0 };

    QueryPerformanceFrequency(&li);

    double frequency = (double)li.QuadPart / 1000.0;

    QueryPerformanceCounter(&li);

    double startTime = li.QuadPart / frequency;
    double lastTime;

    while (window.isOpen())
    {
        QueryPerformanceCounter(&li);

        startTime = li.QuadPart / frequency;

        update(window);
        render(window);

        QueryPerformanceCounter(&li);

        lastTime = li.QuadPart / frequency;

        if (lastTime - startTime < 30.0)
        {
            Sleep(static_cast<DWORD>(lastTime - startTime));
        }
    }

    return 0;
}

void resizeGrid(size_t gridWidth, size_t gridHeight)
{
    g_cells.clear();

    g_gridWidth  = gridWidth;
    g_gridHeight = gridHeight;

    for (size_t i = 0; i < g_gridWidth * g_gridHeight; ++i)
    {
        g_cells[i] = NONE;
    }
}

void update(wd::Window& window)
{
    static float vertSpacing  = g_windowWidth / g_gridWidth;
    static float horizSpacing = g_windowHeight / g_gridHeight;

    MSG msg;

    // Jump through some mad hoops to check if the enter (return) button was pressed
    if (PeekMessageW(&msg, NULL, WM_KEYDOWN, WM_KEYUP, PM_NOREMOVE))
    {
        TranslateMessage(&msg);

        if (msg.message == WM_KEYDOWN) // check if the enter button was pressed
        {
            if (msg.wParam == VK_RETURN)
            {
                g_enterButtonDown = true;
            }
        }

        else if (msg.message = WM_KEYUP) // check if the enter button was released
        {
            if (msg.wParam == VK_RETURN)
            {
                if (g_enterButtonDown) // tell the renderer to update
                {
                    if (g_lastStartIndex < 0 || g_lastEndIndex < 0)
                    {
                        // TODO: render some sort of error message here
                    }

                    else
                    {
                        g_enterButtonDown = false;

                        // calculate the shortest path between the beginning and end vertex
                        g_pathfinder.SetGraphSize(static_cast<uint32_t>(g_gridWidth), static_cast<uint32_t>(g_gridHeight));
                        g_pathfinder.SetFunction(A::Heurisitics::Euclidean);

                        int startX = g_lastStartIndex % g_gridWidth;
                        int startY = g_lastStartIndex / static_cast<int>(g_gridWidth);

                        int endX = g_lastEndIndex % g_gridWidth;
                        int endY = g_lastEndIndex / static_cast<int>(g_gridWidth);

                        auto results = g_pathfinder.CalculateShortestPath(A::Vertex(startX, startY), A::Vertex(endX, endY));

						g_pathDrawn = true;

                        // update the std::map to reflect the result of the shortest path between the beginning and end vertex
                        if (!results.empty())
                        {
                            for (size_t i = 1; i < results.size() - 1; ++i)
                            {
                                auto result  = results[i];
                                size_t index = (result.Y * g_gridWidth) + result.X;

                                g_cells[index] |= VISITED;
                            }
                        }
                    }
                }
            }
        }
    }

    wd::Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case wd::Event::Close:
            window.destroy();
            break;

        case wd::Event::Resize:
        {
            g_windowWidth  = event.size.width;
            g_windowHeight = event.size.height;

            vertSpacing  = g_windowWidth / g_gridWidth;
            horizSpacing = g_windowHeight / g_gridHeight;
        }
        break;

        case wd::Event::MouseDown:
        {
            if (event.mouse.mod & wd::Event::MouseEvent::LEFT)
                g_leftMouseDown = true;

            if (event.mouse.mod & wd::Event::MouseEvent::RIGHT)
                g_rightMouseDown = true;
        }
        break;

        case wd::Event::MouseRelease:
        {
            // if the click was outside of the window, ignore this block
            if (event.mouse.x < 0
                || event.mouse.x > g_windowWidth
                || event.mouse.y < 0
                || event.mouse.y > g_windowHeight)
            {
                break;
            }

            // if enter was pressed, the next time left or right mouse is clicked, clear the grid
            if (g_pathDrawn && (g_rightMouseDown || g_leftMouseDown))
            {
                g_cells.clear();
                g_pathDrawn = false;
			}

            // calculate which square on the grid was clicked
            int column = static_cast<int>(event.mouse.x / horizSpacing);
            int row    = static_cast<int>(event.mouse.y / vertSpacing);

            if (g_leftMouseDown)
            {
                // handle left mouse click -> placing start node
                if (event.mouse.mod & wd::Event::MouseEvent::LEFT)
                {
                    // if there was another square that was the start node, remove the start flag
                    if (g_lastStartIndex != -1)
                        g_cells[g_lastStartIndex] &= ~START;

                    g_leftMouseDown = false;

                    g_lastStartIndex = static_cast<int>(row * g_gridWidth + column);
                    g_cells[g_lastStartIndex] |= START;
                }
            }

            if (g_rightMouseDown)
            {
                // handle right mouse click -> placing end node
                if (event.mouse.mod & wd::Event::MouseEvent::RIGHT)
                {
                    // if there was another square that was the end node, remove it
                    if (g_lastEndIndex != -1)
                        g_cells[g_lastEndIndex] &= ~END;

                    g_rightMouseDown = false;

                    g_lastEndIndex = static_cast<int>(row * g_gridWidth + column);
                    g_cells[g_lastEndIndex] |= END;
                }
            }
        }
        break;
        }
    }
}

void render(const wd::Window& window)
{
    wd::CanvasHandle canvas = window.getCanvasHandle();

    if (!canvas)
        return;

    wd::ColorF lineColor    = wd::ColorF(0.f, 0.f, 0.f);
    wd::ColorF startColor   = wd::ColorF(0.f, .3f, 0.f);
    wd::ColorF endColor     = wd::ColorF(.3f, 0.f, 0.f);
    wd::ColorF visitedColor = wd::ColorF(.001f, .001f, .001f);
    wd::ColorF blockedColor = wd::ColorF(1.f, 1.f, 1.f);

    wd::LineShape line;
    line.color = lineColor;

    float vertSpacing  = g_windowWidth / g_gridWidth;
    float horizSpacing = g_windowHeight / g_gridHeight;

    canvas->beginDraw();
    canvas->clear(wd::Color::WhiteF);

    // draw appropriate rectangles
    for (size_t i = 0; i < g_gridWidth * g_gridHeight; ++i)
    {
        wd::RectShape rect;
        rect.color = wd::Color::BlackF;

        size_t x = i % g_gridWidth;
        size_t y = i / g_gridWidth;

        if (!g_cells[i])
            continue; // if the flag is zero, don't even bother checking for anything else

        rect.dimensions = wd::Vector4f(x * horizSpacing, y * vertSpacing, horizSpacing, vertSpacing);
        rect.filled     = true;

        if (g_cells[i] & START)
        {
            rect.color += startColor;
        }

        if (g_cells[i] & END)
        {
            rect.color += endColor;
        }

        if (g_cells[i] & VISITED)
        {
            rect.color += visitedColor;
        }

        canvas->draw(rect);
    }

    // draw vertical lines
    for (int i = 1; i < g_gridWidth; ++i)
    {
        line.start = wd::Vector2f(vertSpacing * i, 0);
        line.end   = wd::Vector2f(vertSpacing * i, g_windowHeight);

        canvas->draw(line);
    }

    // draw horizontal lines
    for (int i = 1; i < g_gridHeight; ++i)
    {
        line.start = wd::Vector2f(0, horizSpacing * i);
        line.end   = wd::Vector2f(g_windowWidth, horizSpacing * i);

        canvas->draw(line);
    }

    canvas->endDraw();
}