#include "leerenametool.h"
#include "./ui_leerenametool.h"

leeRenameTool::leeRenameTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::leeRenameTool)
{
    ui->setupUi(this);


    connect(ui->browserEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnBrowserChanged);
    connect(ui->browserButton,&QToolButton::clicked, this, &leeRenameTool::OnBrowserClicked);
    connect(ui->NewnameEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnNewNameChanged);
    connect(ui->prefixEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnPrefixChanged);
    connect(ui->SuffixEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnSuffixChanged);
    connect(ui->FilterEdit,&QLineEdit::textChanged, this, &leeRenameTool::OnFilterChanged);
    connect(ui->ReloadTree,&QPushButton::clicked, this, &leeRenameTool::OnReloadTreeView);
    connect(ui->DoRename,&QPushButton::clicked, this, &leeRenameTool::OnDoRenameClicked);
    connect(ui->FilterImage,&QCheckBox::stateChanged,this,&leeRenameTool::OnFilterImagePicture);
    connect(ui->FilterFBX,&QCheckBox::stateChanged,this,&leeRenameTool::OnFilterFBX);

    lpictures <<"*.jpg" <<"*.png" <<"*.tiff";
    l3DFiles << "*.fbx" << "*.obj" << "*.abc";
    lallFiles << "*.*";
    this->setWindowIcon(QIcon("./lee.ico"));
   // ui->browserEdit->setText("C:/Users/LeePhan/Downloads/Kid app/home");
    //ImplanteTreeView("C:/Users/LeePhan/Downloads/Kid app");
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
        ui->browserEdit->setEnabled(false);
        ImplanteTreeView(content);
    }

}

void leeRenameTool::OnNewNameChanged(QString newname)
{
    qDebug() << "Main Name changed" << Qt::endl;
    if(newname =="" || newname.isEmpty() || newname.isNull()) return;
    lnewname = newname;

}

void leeRenameTool::OnPrefixChanged(QString newprefix)
{
    qDebug() << "Prefix changed" << Qt::endl;
    if(newprefix =="" || newprefix.isEmpty() || newprefix.isNull()) return;
    lPrefix = newprefix;

}

void leeRenameTool::OnSuffixChanged(QString newsuffix)
{
    qDebug() << "Suffix changed" << Qt::endl;
    if(newsuffix =="" || newsuffix.isEmpty() || newsuffix.isNull()) return;
    lSuffix = newsuffix;

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

void leeRenameTool::OnReloadTreeView(bool ischecked)
{
    qDebug() << "do reload" << Qt::endl;

}

void leeRenameTool::OnDoRenameClicked(bool isClicked)
{

    if(lnewname.isEmpty() || lnewname.isNull()){

    }

    QStringList files = lDir.entryList(QDir::Files);



    QStringList changedFiles, failedFiles;

    int count=0;
    for(auto &filename : files)
    {
        QFileInfo info(lDir.absolutePath() + "/" + filename);
        QString rawFileName = filename.section(".", 0, 0);
        QString rawSuffix=info.suffix();
        QString countStr = QString::number(count +1);

        QString newName = lnewname == "" ?
                    lCurrentDirName +  "/" + QString::number(count +1) + "."  + rawSuffix :
                    lCurrentDirName +  "/"  + lPrefix + lnewname + lSuffix + countStr +  "."  + rawSuffix
                    ;

        qDebug() << newName << Qt::endl;
        qDebug() << rawSuffix << Qt::endl;
        if(filename !=newName){
            bool success = lDir.rename(info.absoluteFilePath(), newName);

            if(success)
            {
                changedFiles << info.absoluteFilePath();
            }
            else
            {
                failedFiles << info.absoluteFilePath();
            }
        }
        count++;
    }

}

void leeRenameTool::OnFilterImagePicture(int state)
{
    qDebug() << "Make filter Picture image" << Qt::endl;
    if(lCurrentDirName.isEmpty() || lCurrentDirName.isNull()) return;

    QStringList filters = state > 0 ? lpictures : lallFiles;
    fileSystemModel->setNameFilters(filters);
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

