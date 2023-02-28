#include "leerenametool.h"
#include "./ui_leerenametool.h"

leeRenameTool::leeRenameTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::leeRenameTool)
{
    ui->setupUi(this);


    ImplanteSearchAndReplace();
    ui->progressBar->setValue(0);
    connect(ui->browserEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnBrowserChanged);
    connect(ui->browserButton,&QToolButton::clicked, this, &leeRenameTool::OnBrowserClicked);
    connect(ui->NewnameEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnNewNameChanged);
    connect(ui->prefixEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnPrefixChanged);
    connect(ui->SuffixEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnSuffixChanged);
    connect(sline,&QLineEdit::textChanged, this, &leeRenameTool::OnSearchChanged);
    connect(rline,&QLineEdit::textChanged, this, &leeRenameTool::OnReplaceChanged);
    connect(ReplaceBtn,&QPushButton::clicked, this, &leeRenameTool::OnReplaceClicked);
    connect(ui->FilterEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnFilterChanged);
    connect(ui->DoRename,&QPushButton::clicked, this, &leeRenameTool::OnDoRenameClicked);
    connect(ui->FilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ OnFilterBoxChanged(index);});
//    connect(ui->FilterFBX,&QCheckBox::stateChanged,this,&leeRenameTool::OnFilterFBX);

    //ui->browserEdit->setText("C:/Users/LeePhan/Downloads/Kid app/home");


    this->setWindowIcon(QIcon("./icon/lee.ico"));
    //ui->browserEdit->setText("C:/Users/LeePhan/Downloads/Kid app/home");
    lallFiles << "*.*";
    leeFilters.append(lallFiles);
    lpictures <<"*.jpg" <<"*.png" <<"*.tiff" << "*.jpeg" << "*.bmp" << "*.xpm";
    leeFilters.append(lpictures);
    l3DFiles <<"*.fbx" <<"*.obj" <<"*.ma" << "*.mb" << "*.max" << "*.abc" << "*.usd" << "*.uasset";
    leeFilters.append(l3DFiles);
    lPng << "*.png";
    leeFilters.append(lPng);
    ljpg << "*.jpg";
    leeFilters.append(ljpg);
    ljpeg <<"*.jpeg";
    leeFilters.append(ljpeg);

//    leeFilters[2] <<"*.fbx" <<"*.obj" <<"*.ma" << "*.mb" << "*.max" << "*.abc" << "*.usd" << "*.uasset";
//    leeFilters[3] << "*.png";
//    leeFilters[4] << "*.jpg";
//    leeFilters[5] << "*.jpeg";

    //anyLayout->addLayout(ui->SearchReplaceLayout);
}

leeRenameTool::~leeRenameTool()
{
    delete ui;
}

void leeRenameTool::OnBrowserChanged(QString content)
{
    qDebug() << content << Qt::endl;
    QDir dir(content);
    if(dir.exists()){
        lCurrentDirName=content;
        //ui->browserEdit->setEnabled(false);
        ImplanteTreeView(content);
    }

}

void leeRenameTool::OnNewNameChanged(QString newname)
{
    qDebug() << "Main Name changed" << Qt::endl;
    if(newname =="" || newname.isEmpty() || newname.isNull()) return;
    lnewname = newname;
    ReGetValue();
}

void leeRenameTool::OnPrefixChanged(QString newprefix)
{
    qDebug() << "Prefix changed" << Qt::endl;
    if(newprefix =="" || newprefix.isEmpty() || newprefix.isNull()) return;
    lPrefix = newprefix;
    ReGetValue();
}

void leeRenameTool::OnSuffixChanged(QString newsuffix)
{
    qDebug() << "Suffix changed" << Qt::endl;
    if(newsuffix =="" || newsuffix.isEmpty() || newsuffix.isNull()) return;
    lSuffix = newsuffix;
    ReGetValue();

}

void leeRenameTool::OnSearchChanged(QString newSearch)
{
    qDebug() << "newSearch changed" << Qt::endl;
    if(newSearch =="" || newSearch.isEmpty() || newSearch.isNull()) return;
    lSearch = newSearch;
    ReGetValue();
}

void leeRenameTool::OnReplaceChanged(QString newReplace)
{
    qDebug() << "replace changed" << Qt::endl;
    if(newReplace =="" || newReplace.isEmpty() || newReplace.isNull()) return;
    lReplace = newReplace;
    ReGetValue();
}

void leeRenameTool::OnFilterChanged(QString filter)
{
    qDebug() << "Filter changed.." << Qt::endl;
}

void leeRenameTool::OnBrowserClicked()
{
    qDebug() << "browser clicked.." << Qt::endl;
    QFileDialog* fdialog = new QFileDialog();
    fdialog->setFileMode(QFileDialog::Directory);
    fdialog->setOption(QFileDialog::ShowDirsOnly);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "C:/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    qDebug() << dir << Qt::endl;

    if(!dir.isNull() || dir.isEmpty()){
        ui->browserEdit->setText(dir);
        ui->browserEdit->setEnabled(false);
        ImplanteTreeView(dir);
        lCurrentDirName = dir;
        lDir.setPath(dir);
    }

}


void leeRenameTool::OnDoRenameClicked(bool isClicked)
{

    if(lnewname.isEmpty() || lnewname.isNull()){

    }

    QDir nDir(lDir);
    QStringList files = nDir.entryList(QDir::Files);



    QStringList changedFiles, failedFiles;

    int count=0;
    int progres=0;
    ui->progressBar->setRange(0,files.length());
    ui->progressBar->setValue(0);
    for(QString &filename : files)
    {
        QFileInfo info(nDir.absolutePath() + "/" + filename);

        QString rawSuffix=info.suffix();
        QString countStr = QString::number(count +1);
        QString newCopy=lnewname;

        if(newCopy.isEmpty() || newCopy.isNull()) newCopy=filename.section(".",0,0);

        QString newStr= lPrefix + newCopy + lSuffix ;



        QString newName = lCurrentDirName +  "/"  + newStr +  "."  + rawSuffix ;

        newStr = QFile(newName).exists() ?  newStr +  countStr : newStr;

        newName = lCurrentDirName +  "/"  + newStr +  "."  + rawSuffix ;

        qDebug() << newName << Qt::endl;
        //qDebug() << rawSuffix << Qt::endl;

        //bool success ;
        nDir.rename(info.absoluteFilePath(), newName);

//        if(success)
//        {
//            changedFiles << info.absoluteFilePath();
//        }
//        else
//        {
//            failedFiles << info.absoluteFilePath();
//        }
        if(progres > 100)
            progres=100;

        progres++;

        ui->progressBar->setValue(progres);

        count++;
    }
    ui->progressBar->setValue(0);

}

void leeRenameTool::OnFilterBoxChanged(int state)
{
    qDebug() << state << Qt::endl;
    if(lCurrentDirName.isEmpty() || lCurrentDirName.isNull()) return;

    fileSystemModel->setNameFilters(leeFilters[state]);
}



void leeRenameTool::OnFilterFBX(int state)
{
    qDebug() << "3D FBX Only" << Qt::endl;

    QStringList filters = state > 0 ? l3DFiles : lallFiles;
    fileSystemModel->setNameFilters(filters);
    lDir.setNameFilters(filters);

}

void leeRenameTool::OnDirectoryFilterLoader(QString path)
{
    qDebug() << "Directory loaded" << Qt::endl;

}

void leeRenameTool::OnReplaceClicked()
{
    QDir nDir(lDir);
    QStringList files = nDir.entryList(QDir::Files);
    if(files.length() <=0) return;
    int progres=0;
    ui->progressBar->setRange(0,files.length());
    ui->progressBar->setValue(0);
    int count{};
    for(auto &filename : files)
    {
        QFileInfo info(nDir.absolutePath() + "/" + filename);
        //----------------------------
        QRegExp rx("(\\d+)");
        QRegExp rNum("(\\%+$)");
        rNum.indexIn(lSearch,0);
        rx.indexIn(filename,0);

        QString SeachCopy= lSearch;
        QString fileCopy=filename;
        QString FindNumX=rNum.cap(1);
        //lSearch=="%" ? filename.remove(rx) :
        QString num=rx.cap(1),OutNum;

        int nCount = lSearch.count(rNum);
        if(!filename.endsWith(num))
            OutNum=num;

        SeachCopy = SeachCopy.replace(SeachCopy.indexOf("%"),SeachCopy.size(),OutNum.left(nCount));

        //--------------------------------------

        QString nSearch = nCount > 0 ?
                    filename.replace(filename.indexOf(SeachCopy),SeachCopy.size(),lReplace):
                    filename.replace(SeachCopy,lReplace);

        QString Res=filename.section(OutNum,0,OutNum.size());
        //qDebug() << SeachCopy << OutNum <<  FindNumX << Qt::endl;
        QString newName =  lCurrentDirName + "/" + nSearch;

         qDebug() << SeachCopy <<  Res<< Qt::endl;

        if(filename !=newName && !nSearch.isEmpty()){
           nDir.rename(info.absoluteFilePath(), newName);
//          if(!success)  qDebug() <<  Qt::endl;
        }
        if(progres > 100)
            progres=100;

        progres++;

        ui->progressBar->setValue(progres);
        count++;
    }
   ltestDemo();
   // qDebug() << str << Qt::endl;

    ui->progressBar->setValue(0);
    ReGetValue();
}

void leeRenameTool::ReGetValue()
{
    lSuffix = ui->SuffixEdit->text();
    lPrefix = ui->prefixEdit->text();
    lnewname=ui->NewnameEdit->text();
    lSearch=sline->text();
    lReplace=rline->text();
}

QString& leeRenameTool::NumFilter(QString filename,QString &search)
{
    QRegExp rNum("(\\%?)");
    QRegExp rx("(\\d+)");
    rx.indexIn(filename,0);
    QString num= rx.cap(1);
    int nCount = search.count(QLatin1Char('%'));
   // qDebug() << nCount << Qt::endl;

    search=search.replace(search.indexOf("%"),nCount,num.section(rNum,search.indexOf("%"),nCount));
    //qDebug() << search << Qt::endl;
    return search;
}

void leeRenameTool::ImplanteTreeView(QString directory)
{
    QDir dir(directory);
    if(!dir.exists()) return;
    lDir=dir;
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    fileSystemModel->setNameFilterDisables(false);
    connect(fileSystemModel,&QFileSystemModel::directoryLoaded,this,&leeRenameTool::OnDirectoryFilterLoader);
    ui->treeView->setModel(fileSystemModel);
    ui->treeView->setRootIndex(fileSystemModel->setRootPath(directory));
}

void leeRenameTool::ImplanteSearchAndReplace()
{
    spoiler = new Section("Search And Replace",60,this);
    spoiler->toggleButton->setStyleSheet("font: 700 11pt \"Sitka\"; color: rgb(255, 255, 255);");
    spoiler->toggleButton->setAutoRaise(true);
    QVBoxLayout * vlayout = new QVBoxLayout();
    QHBoxLayout* search = new QHBoxLayout();
    vlayout->addLayout(search);
    QLabel * searchLabel = new QLabel("   Search     ",spoiler);
    search->addWidget(searchLabel);
    sline = new QLineEdit(spoiler);
    search->addWidget(sline);
    QHBoxLayout* replace = new QHBoxLayout();
    vlayout->addLayout(replace);
    QLabel * replaceLabel = new QLabel("  Replace  ",spoiler);
    replace->addWidget(replaceLabel);
    rline = new QLineEdit(spoiler);
    rline->setStyleSheet("background-color: rgb(113, 113, 113);");
    sline->setStyleSheet("background-color: rgb(113, 113, 113);");
    replaceLabel->setStyleSheet("font: 700 11pt \"Sitka\";color: rgb(255, 255, 255);");
    searchLabel->setStyleSheet("font: 700 11pt \"Sitka\";color: rgb(255, 255, 255);");
    ReplaceBtn = new QPushButton(spoiler);
    ReplaceBtn->setStyleSheet("font: 700 11pt \"Sitka\";color: rgb(255, 255, 255);background-color: rgb(170, 85, 127);");
    ReplaceBtn->setText("Do Replace");
    vlayout->addWidget(ReplaceBtn);
    replace->addWidget(rline);

    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(5);
    search->setContentsMargins(0,0,0,0);
    search->setSpacing(0);
    replace->setContentsMargins(0,0,0,0);
    replace->setSpacing(5);

    spoiler->setContentLayout(*vlayout);
    ui->VLayout->insertWidget(5,spoiler);

}

QString leeRenameTool::lRemoveAllDigit(QString iStr)
{
    QRegExp rx("(\\d+)");
    QString outStr,mapStr;
    int pos=0;
    while((pos=rx.indexIn(iStr,pos)) !=-1)  {
        outStr += rx.cap(1);
        pos+=rx.matchedLength();
    }
    return outStr;
}

QString leeRenameTool::ltestDemo()
{
    QString demoStr="_545345435_asdasd";
    QString temp="_%_",reMap;;

    QString mapStr = lRemoveAllDigit(demoStr);
    QString result = temp.replace("%",mapStr);

    QString endResult=demoStr.replace(result,"taoday_");
    qDebug() << endResult << Qt::endl;
//    for(auto s:endResult){
//        qDebug() << s << Qt::endl;
//    }
    //qDebug() << mapStr << endResult.length() << result << Qt::endl;
    return "";
}

