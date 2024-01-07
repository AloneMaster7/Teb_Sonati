#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QLayout>
#include <QDir>
#include <QFont>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class Widget : public QWidget
{
    Q_OBJECT
    QVBoxLayout* mainLayout;
    bool DocOpen;
    QFont* Title,*Text;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    Ui::Widget *ui;
    void BuildHeartProblems();
    void BuildBrainProblems();
    void BuildDigesionProblems();
    void BuildSexProblems();
    void BuildMentalProblems();
    void BuildBreathingProblems();
    void BuildKidneyProblems();
    void BuildLiverProblems();
    void BuildHomeButton(QVBoxLayout*);
    void SetDefault();
private slots:
    void Back();
    void BuildMainPage();
    void ItemClicked(QListWidgetItem*);
    void ShowContent(QListWidgetItem*);
    void BuildAboutPage();
    void BuildSettingPage();
};
#endif // WIDGET_H
