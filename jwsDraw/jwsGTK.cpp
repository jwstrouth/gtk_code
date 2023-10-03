#include "jwsGTK.h"

cairo_surface_t *jwsGTK::m_surface = 0;
double jwsGTK::m_start_x = 0;
double jwsGTK::m_start_y = 0;
jwsGTK *pGTK = 0;

jwsGTK::jwsGTK()
{
    m_app = gtk_application_new ("james.strouth.drawexa", G_APPLICATION_DEFAULT_FLAGS);
    pGTK = this;
}

jwsGTK::~jwsGTK()
{
    g_object_unref(m_app);
}

void jwsGTK::Connect()
{
    g_signal_connect(m_app, "activate", G_CALLBACK(Activate), NULL);
}

int jwsGTK::Run(int argc, char **argv)
{
    int status = 0;

    Connect();
    status = g_application_run(G_APPLICATION(m_app), argc, argv);

    return status;
}

void jwsGTK::Activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget   *window;
    GtkWidget   *frame;
    GtkWidget   *drawing_area;
    GtkGesture  *drag;
    GtkGesture  *press;

    if(pGTK)
    {

        window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "JWS Drawing Area");

        g_signal_connect(window, "destroy", G_CALLBACK(CloseWindow), NULL);

        frame = gtk_frame_new(NULL);
        gtk_window_set_child(GTK_WINDOW(window), frame);

        drawing_area = gtk_drawing_area_new();

        gtk_widget_set_size_request(drawing_area, 800, 600);

        gtk_frame_set_child(GTK_FRAME(frame), drawing_area);

        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), DrawCB, NULL, NULL);

        g_signal_connect_after(drawing_area, "resize", G_CALLBACK(ResizeCB), NULL);

        drag = gtk_gesture_drag_new();
        gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
        gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(drag));
        g_signal_connect(drag, "drag-begin", G_CALLBACK(DragBegin), drawing_area);
        g_signal_connect(drag, "drag-update", G_CALLBACK(DragUpdate), drawing_area);
        g_signal_connect(drag, "drag-end", G_CALLBACK(DragEnd), drawing_area);

        //gtk_widget_show(window);
        gtk_widget_set_visible(window, true);

    }
}

void jwsGTK::CloseWindow(void)
{
    if(pGTK)
    {
        if(pGTK->m_surface)
        {
            cairo_surface_destroy(pGTK->m_surface);
        }
    }
}

void jwsGTK::DrawCB(GtkDrawingArea *drawing_area,
                    cairo_t        *cr,
                    int             width,
                    int             height,
                    gpointer        data)
{
    if(pGTK)
    {
        cairo_set_source_surface(cr, pGTK->m_surface, 0, 0);
        cairo_paint(cr);
    }
}

void jwsGTK::ResizeCB(GtkWidget *widget,
                      int        width,
                      int        height,
                      gpointer   data)
{
    if(pGTK)
    {
        if(pGTK->m_surface)
        {
            cairo_surface_destroy(pGTK->m_surface);
            pGTK->m_surface = NULL;
        }

        if(gtk_native_get_surface(gtk_widget_get_native(widget)))
        {
            pGTK->m_surface = gdk_surface_create_similar_surface(gtk_native_get_surface(gtk_widget_get_native(widget)),
                                                                                        CAIRO_CONTENT_COLOR,
                                                                                        gtk_widget_get_width(widget),
                                                                                        gtk_widget_get_height(widget));

            /* Initialize the surface to white */
            ClearSurface();
        }
    }
}

void jwsGTK::ClearSurface(void)
{
    cairo_t *cr;

    if(pGTK)
    {
        cr = cairo_create(pGTK->m_surface);

        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_paint(cr);

        cairo_destroy(cr);
    }
}

void jwsGTK::DragBegin(GtkGestureDrag *gesture,
                       double          x,
                       double          y,
                       GtkWidget      *area)
{
    if(pGTK)
    {
        pGTK->m_start_x = x;
        pGTK->m_start_y = y;

        DrawBrush(area, x, y);
    }
}

void jwsGTK::DragUpdate(GtkGestureDrag *gesture,
                        double          x,
                        double          y,
                        GtkWidget      *area)
{
    if(pGTK)
    {
        DrawBrush(area, pGTK->m_start_x + x, pGTK->m_start_y + y);
    }
}

void jwsGTK::DragEnd(GtkGestureDrag *gesture,
                     double          x,
                     double          y,
                     GtkWidget      *area)
{
    if(pGTK)
    {
        DrawBrush(area, pGTK->m_start_x + x, pGTK->m_start_y + y);
    }
}

void jwsGTK::DrawBrush(GtkWidget *widget,
                       double     x,
                       double     y)
{
    cairo_t *cr;

    if(pGTK)
    {
        /* Paint to the surface, where we store our state */
        cr = cairo_create(pGTK->m_surface);

        cairo_rectangle(cr, x - 3, y - 3, 6, 6);
        cairo_fill(cr);

        cairo_destroy(cr);

        /* Now invalidate the drawing area. */
        gtk_widget_queue_draw(widget);
    }
}


