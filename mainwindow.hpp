#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class QSignalMapper;
class QUndoStack;
class QSlider;
class Scene;

static constexpr const char* APP_VERSION = "0.2";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void zoomIn();
    void zoomOut();
    void setZoom(const int &value);
    void updateActions();

private:
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool maybeSave();
    void setCurrentFile(const QString &fileName = QString());
    void createZoom();
    void connectActions();
    void loadComponents();

private:
    Ui::MainWindow *_ui;
    Scene *_scene;
    QSlider *_zoomSlider;
    QSignalMapper *_mruMapper;
    QString _fileName;
};
#endif // MAINWINDOW_HPP
