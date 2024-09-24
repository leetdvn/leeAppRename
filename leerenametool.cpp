#include "leerenametool.h"
#include "./ui_leerenametool.h"
#include "LeeDefination.h"

leeRenameTool::leeRenameTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::leeRenameTool)
{
    ui->setupUi(this);

    undo_stack = new QUndoStack(this);
    setFocusPolicy(Qt::StrongFocus);

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
    connect(ui->DoRename,&QPushButton::clicked, [&](bool Clicked){
        OnDoRenameClicked(Clicked);
        //leeCommand = new LeeTdCommand("",QStringList(),QStringList());
    });
    connect(ui->UndoBtn,&QPushButton::clicked,this,&leeRenameTool::OnUndo_Clicked);
    connect(ui->FilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ OnFilterBoxChanged(index);});
//    connect(ui->FilterFBX,&QCheckBox::stateChanged,this,&leeRenameTool::OnFilterFBX);

    //ui->browserEdit->setText("C:/Users/THANGLE/Desktop/hoatietnen");


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

#pragma region //PUBLIC SLOTS

void leeRenameTool::OnBrowserChanged(QString content)
{
    //qDebug() << content << Qt::endl;
    QDir dir(content);
    if(!dir.exists()) {
        LEELOG("dir does'nt exists.");
        return;
    }
    dir.setFilter(QDir::Files| QDir::Hidden);
    dir.setSorting(QDir::Name);

    QFileInfoList names = dir.entryInfoList(QDir::Files,QDir::Name);

    if(defaultnametiles.length() > 0)
        defaultnametiles = QStringList();

    for(auto f : names){
        qDebug() << "file : " << f.filePath() << Qt::endl;
        defaultnametiles.append(f.filePath());
    }

    lCurrentDirName=content;
    //ui->browserEdit->setEnabled(false);
    ImplanteTreeView(content);
    Currentnamefiles = *&defaultnametiles;
}

void leeRenameTool::OnNewNameChanged(QString newname)
{
    LEELOG("Main Name changed")
    if(newname =="" || newname.isEmpty() || newname.isNull()) return;
    lnewname = newname;
    ReGetValue();
}

void leeRenameTool::OnPrefixChanged(QString newprefix)
{
    LEELOG("Prefix changed")
    if(newprefix =="" || newprefix.isEmpty() || newprefix.isNull()) return;
    lPrefix = newprefix;
    ReGetValue();
}

void leeRenameTool::OnSuffixChanged(QString newsuffix)
{
    LEELOG("Suffix changed")
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

    //if(lnewname.isEmpty() || lnewname.isNull()){

    //    qDebug() << "null " << Qt::endl;
    //    return;
    //}

    qDebug() << step << Qt::endl;
    QDir nDir(lDir);
    QStringList files = nDir.entryList(QDir::Files);
    if(files.isEmpty()){
        qDebug() << "empty file in folder " << Qt::endl;
        return;
    }

    QStringList changedFiles;

    int count=0;
    int progres=0;
    ui->progressBar->setRange(0,files.length());
    ui->progressBar->setValue(0);

    QString LastNameChanged;
    for(QString &filename : files)
    {
        QFileInfo info(nDir.absolutePath() + "/" + filename);

        QString rawSuffix=info.suffix();

        QString fName = filename.replace("." + rawSuffix,"");
        //get new input name
        QString newName = GetInputName(count +1,fName) + "." + rawSuffix;

        //create New Name

        newName = nDir.absolutePath() +  "/"  +  newName;//ui->NewnameEdit->text() == "" ?

        qDebug() << "Info :" << GetInputName(count +1,fName) << "File : "<< fName <<  Qt::endl;

        //bool success ;
        //nDir.rename(info.absoluteFilePath(), newName);

        changedFiles.append(newName);

        LastNameChanged =newName;
        if(progres > 100)
            progres=100;

        progres++;

        ui->progressBar->setValue(progres);

        count++;
    }

    ui->progressBar->setValue(0);

    InitCommand(nDir.absolutePath(),defaultnametiles,changedFiles);

}

#pragma endregion //SLOTS

QString leeRenameTool::GetInputName(int inIdx,QString inOldFilename)
{
    QString NewName= ui->NewnameEdit->text() == "" ?
                          inOldFilename + ui->SuffixEdit->text() :
                          ui->NewnameEdit->text() + ui->SuffixEdit->text();
    NewName = ui->prefixEdit->text() + NewName ;

    NewName = NewName.isEmpty() || NewName.isNull() ? inOldFilename : NewName;

    if(IsValidName(ValidNames,NewName)){
        NewName += QString::number(inIdx);
    }
    else{
        ValidNames.push_back(NewName);
    }


    return NewName;
}

QStringList leeRenameTool::GetFileNames()
{
    if(step==0) return defaultnametiles;

    return processNames[step];
}

bool leeRenameTool::IsValidName(const QStringList inValidNames, const QString inNewName)
{
    if(inValidNames.isEmpty()) return false;

    for(auto n : inValidNames)
        if(n==inNewName) return true;

    return false;
}

void leeRenameTool::OnFilterBoxChanged(int state)
{
    LEELOG(state);
    if(lCurrentDirName.isEmpty() || lCurrentDirName.isNull()) return;

    fileSystemModel->setNameFilters(leeFilters[state]);
}

void leeRenameTool::OnUndo_Clicked()
{
    undo_stack->undo();
    step--;
    if(step==0){
        ui->UndoBtn->setEnabled(false);
        return;
    }

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
    //qDebug() << "Directory loaded" << Qt::endl;
    LEELOG("load Directory : " + path);
}

void leeRenameTool::OnReplaceClicked()
{
    if(sline->text().isEmpty() || rline->text().isEmpty() ) {
        //qDebug() << "empty Search and replace" << Qt::endl;
        LEELOG("empty Search and replace");
        return;
    }

    if(!processNames.empty())
        ui->UndoBtn->setEnabled(true);

    QDir nDir(lDir);
    QStringList files = nDir.entryList(QDir::Files);
    QStringList NewNames,OldNames;
    if(files.length() <=0) return;
    int progres=0;
    ui->progressBar->setRange(0,files.length());
    ui->progressBar->setValue(0);
    for(auto &filename : files)
    {
        QFileInfo info(nDir.absolutePath() + "/" + filename);
        //----------------------------
        QRegularExpression rx("(\\d+)");
        QRegularExpression rNum("(\\%+$)");
        rNum.match(sline->text(),0);
        rx.match(filename,0);

        QString SeachCopy= sline->text();
        rNum.match(filename).captured();
        //lSearch=="%" ? filename.remove(rx) :
        QString num=rx.match(filename).captured(),OutNum;

        int nCount = sline->text().count(rNum);
        if(!filename.endsWith(num))
            OutNum=num;

        SeachCopy = SeachCopy.replace(SeachCopy.indexOf("%"),SeachCopy.size(),OutNum.left(nCount));

        //--------------------------------------

        QString nSearch = nCount > 0 ?
                    filename.replace(filename.indexOf(SeachCopy),SeachCopy.size(),lReplace):
                    filename.replace(SeachCopy,lReplace);

        filename.section(OutNum,0,OutNum.size());
        //qDebug() << SeachCopy << OutNum <<  FindNumX << Qt::endl;
        QString newName =  lCurrentDirName + "/" + nSearch;

        qDebug() << "Info :" << newName <<  Qt::endl;
        NewNames.append(newName);
        OldNames.append(info.absoluteFilePath());

        if(progres > 100)
            progres=100;

        progres++;

        ui->progressBar->setValue(progres);
    }
   // qDebug() << str << Qt::endl;

    ui->progressBar->setValue(0);
    ReGetValue();

    InitCommand(nDir.absolutePath(),OldNames,NewNames);
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
    QRegularExpression rNum("(\\%?)");
    QRegularExpression rx("(\\d+)");
    rx.match(filename);// indexIn(filename,0);
    QString num= rx.match(filename).captured();// cap(1);
    int nCount = search.count(QLatin1Char('%'));
   // qDebug() << nCount << Qt::endl;

    search=search.replace(search.indexOf("%"),nCount,num.section(rNum,search.indexOf("%"),nCount));
    //qDebug() << search << Qt::endl;
    return search;
}

void leeRenameTool::keyPressEvent(QKeyEvent *event)
{
    if(event->key()== Qt::CTRL && event->key() == Qt::Key_Z ){
        qDebug() << "abac" << Qt::endl;
        event->accept();
    }
    //qDebug() << "\nkey event in board: " << event->key() << Qt::endl;
    QWidget::keyPressEvent(event);
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

void leeRenameTool::InitCommand(const QString inDir,const QStringList inOldNames,const QStringList inNewNames)
{
    if(inDir.isEmpty() || inDir.isNull() || inOldNames.isEmpty() || inNewNames.isEmpty()){
        LEELOG("Input Error..");
        return;
    }

    LeeTdCommand* command = new LeeTdCommand(inDir,inOldNames,inNewNames);
    undo_stack->push(command);
    ui->UndoBtn->setEnabled(true);
    step++;
}

QString leeRenameTool::lRemoveAllDigit(QString iStr)
{
    QRegularExpression rx("(\\d+)");
    QString outStr;
    int pos=0;
    while(rx.match(iStr).hasMatch())  {
        outStr += rx.match(iStr).captured();
        pos+=rx.match(iStr).capturedLength();
    }
    return outStr;
}

QString leeRenameTool::ltestDemo()
{
    QString demoStr="_545345435_asdasd";
    QString temp="_%_";;

    QString mapStr = lRemoveAllDigit(demoStr);
    QString result = temp.replace("%",mapStr);

    QString endResult=demoStr.replace(result,"taoday_");
    LEELOG(endResult);
    //qDebug() << endResult << Qt::endl;
//    for(auto s:endResult){
//        qDebug() << s << Qt::endl;
//    }
    //qDebug() << mapStr << endResult.length() << result << Qt::endl;
    return "";
}
