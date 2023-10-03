#ifndef JWSGTK_H
#define JWSGTK_H

#include <gtk/gtk.h>

class jwsGTK
{
    public:
        jwsGTK();
        virtual ~jwsGTK();

        void Connect();
        int Run(int argc, char **argv);

        static void Activate(GtkApplication *app, gpointer user_data);
        static void CloseWindow();
        static void DrawCB(GtkDrawingArea *drawing_area,
                           cairo_t        *cr,
                           int             width,
                           int             height,
                           gpointer        data);
        static void ResizeCB(GtkWidget *widget,
                             int        width,
                             int        height,
                             gpointer   data);
        static void ClearSurface(void);
        static void DragBegin(GtkGestureDrag *gesture,
                              double          x,
                              double          y,
                              GtkWidget      *area);
        static void DragUpdate(GtkGestureDrag *gesture,
                               double          x,
                               double          y,
                               GtkWidget      *area);
        static void DragEnd(GtkGestureDrag *gesture,
                            double          x,
                            double          y,
                            GtkWidget      *area);
        static void DrawBrush(GtkWidget *widget,
                              double     x,
                              double     y);

        /* Surface to store current scribbles */
        static cairo_surface_t *m_surface;
        static double m_start_x;
        static double m_start_y;

    protected:

    private:
        GtkApplication *m_app;
};

#endif // JWSGTK_H
