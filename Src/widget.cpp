#include "widget.h"
#include "ui_widget.h"
#include <QtWidgets>
#include <QFileInfoList>
#include <QDir>
#include <QLayout>
#include <QFontDialog>

//#50C4D3 //hover
//#126E83 //selection
//#D8E3E7 //back
//#132C33 //text

QString subjects[] = {"بیماری های قلبی","بیماری های مغزی","بیماری های گوارشی","بیماری های جنسی","بیماری های روحی روانی","بیماری های تنقسی","بیماری های کلیوی","بیماری های کبدی"};
QString icons[] = {"Heart","Brain","Digesion","Sex","Mental","Breathing","Kidney","Liver"};
void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("icon.ico"));
    /*this->setStyleSheet("QWidget,QListWidget{background-color: white;"
                        "background-attachment: scroll;"
                        "alternate-background-color: yellow;}"
                        "QListView::item{padding: 20px;background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0.1 #ffffff, stop: 1 #000000);}"
                        "QListView::item:hover{padding: 10px;background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0.1 #ffffff, stop: 1 #000000);color: green;}");*/
    /*this->setStyleSheet("QWidget,QListWidget{background-color: white;"
                            "background-attachment: scroll;"
                            "alternate-background-color: yellow;}"
                            "QListView::item{padding: 20px;background: white;color: #00aaff}"
                            "QListView::item:hover{padding: 10px;background: #00aaff;color: #ffffff;}"
                        "QListView::item:selected{padding: 10px;background: #00aaff;color: #ffffff;}");*/
    this->setStyleSheet("QWidget{color: #132C33;}"
                        "QListWidget{border : 2px solid #132C33;border-radius: 5px;background-color: white;"
                            "background-attachment: scroll;"
                            "alternate-background-color: yellow;}"
                        "QListView::item{padding: 20px;background: white;}"
                        "QListView::item:hover{padding: 10px;background: #50C4D3;color: #ffffff;}"
                        "QListView::item:selected{padding: 10px;background: #126E83;color: #ffffff;}"
                        "QLabel{background-color: #D8E3E7;border : 2px solid #132C33;border-radius: 5px;padding: 5px;}"
                        "QPushButton{background-color: white;}"
                        "QListWidget QScrollBar{background : #132C33;}"
                        "QPushButton:hover{background-color: #D8E3E7;border-radius: 5px}"
                        "QScrollArea{border : 2px solid #132C33;border-radius: 5px;}"
                        "QScrollArea QScrollBar{background : #132C33;border-radius: 5px;}");
    this->Title = nullptr;
    this->Text = nullptr;
    this->SetDefault();
    this->BuildMainPage();
    this->DocOpen = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::BuildMainPage()
{
    if(this->layout()){
        clearLayout(this->layout());
        QDir::setCurrent("..");
        delete this->layout();
    }

    QVBoxLayout* l = new QVBoxLayout;
    this->setLayout(l);

    QHBoxLayout* hl = new QHBoxLayout;
    QPushButton* about = new QPushButton;
    about->setIcon(QIcon("About.png"));
    about->setIconSize(QSize(40,40));
    about->setFlat(true);
    about->setCursor(Qt::PointingHandCursor);
    hl->addWidget(about,0,Qt::AlignLeft);
    QPushButton* setting = new QPushButton;
    setting->setIcon(QIcon("Setting.png"));
    setting->setIconSize(QSize(40,40));
    setting->setFlat(true);
    setting->setCursor(Qt::PointingHandCursor);
    hl->addWidget(setting,1,Qt::AlignLeft);
    l->addLayout(hl,0);
    QObject::connect(about,SIGNAL(clicked()),this,SLOT(BuildAboutPage()));
    QObject::connect(setting,SIGNAL(clicked()),this,SLOT(BuildSettingPage()));

    QListWidget* lw = new QListWidget;
    lw->setIconSize(QSize(100,100));
    l->addWidget(lw);
    QListWidgetItem* lwi;
    int j=0;
    for(QString& i:subjects){
        lwi = new QListWidgetItem;
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
        QIcon* ic = new QIcon(icons[j++]);
        lwi->setIcon(*ic);
    }
    //lw->setFlow(QListView::LeftToRight);
    lw->setFocusPolicy(Qt::NoFocus);
    this->mainLayout = l;
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem *)),
                     this,SLOT(ItemClicked(QListWidgetItem *)));
}

void Widget::BuildHeartProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Heart");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Heart Problems Data Not Found!");
        return;
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildBrainProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Brain");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Brain Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildDigesionProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Digesion");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Digesion Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildSexProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Sex");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Sex Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildMentalProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Mental");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Mental Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildBreathingProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Breathing");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Breathing Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildKidneyProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Kidney");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Kidney Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildLiverProblems()
{
    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;

    QDir d(QDir::current().path()+"/Liver");
    if(!d.exists()){
        QMessageBox::critical(nullptr,"","Liver Problems Data Not Found!");
    }
    QDir::setCurrent(d.path());
    this->BuildHomeButton(l);
    QStringList li = d.entryList(QStringList() << "*.txt",QDir::Files);
    QListWidget* lw = new QListWidget;
    QListWidgetItem* lwi = new QListWidgetItem;
    for(QString& i:li){
        lwi = new QListWidgetItem;
        i.resize(i.lastIndexOf('.'));
        lwi->setText(i);
        lwi->setTextAlignment(Qt::AlignCenter);
        lwi->setFont(*this->Title);
        lw->addItem(lwi);
    }
    l->addWidget(lw);
    lw->setFocusPolicy(Qt::NoFocus);
    lw->setCursor(Qt::PointingHandCursor);

    QObject::connect(lw,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(ShowContent(QListWidgetItem*)));
    this->setLayout(l);
}

void Widget::BuildHomeButton(QVBoxLayout *l)
{
    QPushButton* home = new QPushButton();
    QIcon* ic = new QIcon("..\\home.png");
    home->setIcon(*ic);
    home->setFlat(true);
    home->setIconSize(QSize(40,40));
    l->addWidget(home,0,Qt::AlignTop|Qt::AlignLeft);
    QObject::connect(home,SIGNAL(clicked()),this,SLOT(Back()));
    home->setCursor(Qt::PointingHandCursor);
}

void Widget::SetDefault()
{
    if(this->Title)
        delete this->Title;
    this->Title = new QFont("YEKAN PLUS",18);
    if(this->Text)
        delete this->Text;
    this->Text = new QFont("YEKAN PLUS",16,3,false);
}

void Widget::Back()
{
    if(this->DocOpen){
        QString p = QDir::currentPath();
        QDir::setCurrent("..");
        if(p.endsWith(icons[0]))
            this->BuildHeartProblems();
        else if(p.endsWith(icons[1]))
            this->BuildBrainProblems();
        else if(p.endsWith(icons[2]))
            this->BuildDigesionProblems();
        else if(p.endsWith(icons[3]))
            this->BuildSexProblems();
        else if(p.endsWith(icons[4]))
            this->BuildMentalProblems();
        else if(p.endsWith(icons[5]))
            this->BuildBreathingProblems();
        else if(p.endsWith(icons[6]))
            this->BuildKidneyProblems();
        else if(p.endsWith(icons[7]))
            this->BuildLiverProblems();
    }else{
        this->BuildMainPage();
    }
    this->DocOpen = false;
}

void Widget::ItemClicked(QListWidgetItem *i)
{
    QString s = i->text();
    if(s==subjects[0])
        this->BuildHeartProblems();
    else if(s==subjects[1])
        this->BuildBrainProblems();
    else if(s==subjects[2])
        this->BuildDigesionProblems();
    else if(s==subjects[3])
        this->BuildSexProblems();
    else if(s==subjects[4])
        this->BuildMentalProblems();
    else if(s==subjects[5])
        this->BuildBreathingProblems();
    else if(s==subjects[6])
        this->BuildKidneyProblems();
    else if(s==subjects[7])
        this->BuildLiverProblems();
}

void Widget::ShowContent(QListWidgetItem* it)
{
    QString FileName = it->text()+".txt";
    QFile f(QDir::current().path()+"\\"+FileName);
    if(!f.open(QFile::ReadOnly)){
        QMessageBox::information(nullptr,"Error : File does not open",
                                 "Error : File does not open");
        return;
    }
    QByteArray* ba = new QByteArray(f.readAll());
    QString s(*ba);

    clearLayout(this->layout());
    delete this->layout();
    QVBoxLayout* l = new QVBoxLayout;
    this->BuildHomeButton(l);

    QLabel* lbl = new QLabel(s);
    lbl->setStyleSheet("padding: 40px");
    lbl->setFont(*this->Text);
    lbl->setWordWrap(true);
    lbl->setTextFormat(Qt::AutoText);
    lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    //TextSelectableByMouse | LinksAccessibleByMouse | LinksAccessibleByKeyboard

    QScrollArea* sa = new QScrollArea;
    sa->setWidget(lbl);
    sa->setWidgetResizable(true);
    l->addWidget(sa);
    l->setAlignment(Qt::AlignRight);
    this->setLayout(l);
    this->DocOpen = true;
}

void Widget::BuildAboutPage()
{
    QWidget* w = new QWidget;
    w->setParent(this,Qt::Window);
    w->setWindowModality(Qt::WindowModal);
    w->setMaximumSize(QSize(400,600));
    w->setStyleSheet(this->styleSheet());
    QVBoxLayout* l = new QVBoxLayout;
    QString c = "نرم افزار طب سنتی اسلامی<hr/>"
                "این نرم افزار توسط جواد رحیمی ساخته و برنامه نویسی شده است.<br/>"
                "این نرم افزار برای معرفی راهکار های طب سنتی اسلامی برای انواع بیماری ها ساخته شده است.<br/>"
                "* این صرفا یک نمونه کار است *"
                "<hr>+989391745806<br>"
                "rsl.nbri@iran.ir";
    QLabel* lbl = new QLabel(c);
    lbl->setFont(*this->Text);
    lbl->setAlignment(Qt::AlignTop);
    lbl->setWordWrap(true);
    lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    l->addWidget(lbl);
    w->setLayout(l);
    w->show();
}

void Widget::BuildSettingPage()
{
    QWidget* w = new QWidget(this,Qt::Window);
    //w->setParent(this,Qt::Window);
    w->setWindowModality(Qt::WindowModal);
    w->setMaximumSize(QSize(400,400));
    w->setStyleSheet(this->styleSheet());

    QVBoxLayout* l = new QVBoxLayout;

    QScrollArea* sa = new QScrollArea;
    QVBoxLayout* sal = new QVBoxLayout;

    QPushButton* lbl = new QPushButton("تنظیمات");
    lbl->setFont(*this->Text);
    lbl->setFlat(true);
    lbl->setStyleSheet("background-color: white;");
    sal->addWidget(lbl);

    QPushButton* lbl1 = new QPushButton("انتخاب فونت نوشته");
    lbl1->setFont(*this->Text);
    sal->addWidget(lbl1);
    QObject::connect(lbl1,&QPushButton::clicked,[this](){
        bool ok;
        QFont f = QFontDialog::getFont(&ok,this);
        if(ok){
            this->Text = new QFont(f);
            f.setWeight(f.weight()+6);
            this->Title = new QFont(f);
        }
        clearLayout(this->layout());
        delete this->layout();
        emit BuildMainPage();
    });

    sa->setLayout(sal);
    l->addWidget(sa);

    w->setLayout(l);
    w->show();
}
