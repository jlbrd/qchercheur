#include "widget.h"
#include "ui_widget.h"
#include "javahighlighter.h"
#include "xmlhighlighter.h"
#include "sqlhighlighter.h"
#include "highlighter.h"
#include "mainwindow.h"
#include "find.h"
#include "settings.h"
#include "cherchefichier.h"

#include <QFileDialog>
#include <QDirIterator>
#include <QDebug>
#include <QTextStream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QMessageBox>
#include <QBrush>
#include <QMenu>
#include <QClipboard>
#include <QProcess>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextCodec>
#include <QStackedWidget>
#include <QCompleter>
#include <QFileSystemModel>
#include <QElapsedTimer>
#include <QThread>


Widget::Widget(QWidget *parent, QString chemin, QString texte) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    highlighter = 0;
    textApercu = (TextEdit*) ui->apercu->widget(0);
    finder = new Find(ui->findWidgetContainer, this, textApercu);
    ui->tableResultats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWindowFlags(Qt::Widget);
    ui->info->setText("");
    ui->nbOccurrences->setText("");
    ui->comboRepertoires->addItems(Settings::repertoires());
    if(chemin != "") {
        ui->comboRepertoires->setCurrentText(chemin);        
    } else {
        ui->comboRepertoires->setCurrentIndex(-1);
    }
    if(texte != "") {
        ui->comboContenant->setCurrentText(texte);
    }
    connect(textApercu, SIGNAL(nouvelleRecherche(QString)), this, SLOT(nouvelleRecherche(QString)));

    QCompleter *completer = ui->comboRepertoires->completer();
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    QFileSystemModel *fsModel = new QFileSystemModel(completer);
    fsModel->setRootPath(ui->comboRepertoires->currentText());
    completer->setModel(fsModel);
    completer->setWrapAround(true);
    completer->setMaxVisibleItems(15);
    nbThread = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_boutonDepart_clicked()
{
    //QElapsedTimer timer;
    //timer.start();
    stopper = false;
    ui->boutonDepart->setEnabled(false);
    ui->tableResultats->setRowCount(0);
    ui->tableResultats->clearContents();
    textApercu->setPlainText("");
    ui->nbOccurrences->setText("");
    Settings::ajouterRepertoires(repertoire());
    QApplication::processEvents(QEventLoop::AllEvents);
    QDirIterator *it;
    if(ui->comboNomFichier->currentText() != "") {
        QStringList filtreNomFichiers = ui->comboNomFichier->currentText().replace(" ","").split(";");
        qDebug() << filtreNomFichiers;
        it = new QDirIterator(ui->comboRepertoires->currentText(), filtreNomFichiers, QDir::NoFilter, QDirIterator::Subdirectories);
    } else {
        it = new QDirIterator(ui->comboRepertoires->currentText(), QDir::NoFilter, QDirIterator::Subdirectories);
    }
    while (!stopper && it->hasNext()) {
        if(nbThread > 30) {
            continue;
        }
        QString nomFichier = it->next();
        //qDebug() << nomFichier;
        ui->info->setText(QDir::toNativeSeparators(QFileInfo(nomFichier).absoluteDir().absolutePath()));
        if(repertoireExclu(nomFichier)) {
            continue;
        }
        else if(ui->comboContenant->currentText() == "" ) {
            fichierTrouve(nomFichier);
        } else if(!nomFichier.isEmpty()){
                Cherchefichier *cherche = new Cherchefichier(nomFichier, regularExpression());
                QThread * th = new QThread();
                nbThread++;
                cherche->moveToThread(th);
                connect(th, SIGNAL(finished()), th, SLOT(deleteLater()), Qt::DirectConnection);
                connect(th, SIGNAL(finished()), this,   SLOT(threadFinished()),      Qt::DirectConnection);
                connect(cherche, SIGNAL(finished()), th, SLOT(quit()),        Qt::DirectConnection);
                connect(cherche, SIGNAL(finished()), cherche, SLOT(deleteLater()), Qt::DirectConnection);
                connect(cherche, &Cherchefichier::fichierTrouve, this, &Widget::fichierTrouve);
                connect(th, &QThread::started, cherche, &Cherchefichier::chercher, Qt::DirectConnection);
                th->start();
        }
        QApplication::processEvents(QEventLoop::AllEvents);
    }
    delete it;
    ui->boutonDepart->setEnabled(true);
    ui->info->setText("");
    stopper = false;
    //qDebug() << "elapsed" << timer.elapsed();
}

void Widget::threadFinished() {
    nbThread--;
}

bool Widget::repertoireExclu(QString nomFichier) {
    QStringList exclusions = ui->comboExclusion->currentText().replace(" ","").split(";", Qt::SkipEmptyParts);
    foreach (QString exclusion, exclusions) {
        if(nomFichier.contains(exclusion)) {
            return true;
        }
    }
    return false;
}

void Widget::fichierTrouve(const QString &nomFichier) {
    ui->tableResultats->insertRow(ui->tableResultats->rowCount());
    ui->tableResultats->setRowCount(ui->tableResultats->rowCount());
    int numLigne = ui->tableResultats->rowCount()-1;
    ui->tableResultats->setItem(numLigne, 0, new QTableWidgetItem(nomFichier.section("/", -1)));
    ui->tableResultats->setItem(numLigne, 1, new QTableWidgetItem(nomFichier));
    ui->tableResultats->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

QRegularExpression Widget::regularExpression() {
   QString recherche = ui->comboContenant->currentText().replace("(", "\\(");
    QRegularExpression::PatternOptions options = QRegularExpression::CaseInsensitiveOption;
    if(ui->respecterCasse->isChecked()) {
        options = QRegularExpression::NoPatternOption;
    }
    if(ui->motEntier->isChecked()) {
        recherche = "\\b" + recherche +"\\b";
    }
    return QRegularExpression(recherche, options);
}

void Widget::on_boutonArret_clicked()
{
    stopper = true;
}

void Widget::on_changerRepertoire_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose catalog"), ui->comboRepertoires->currentText(),  QFileDialog::ShowDirsOnly
                                                     | QFileDialog::ReadOnly);
    ui->comboRepertoires->setCurrentText(path);
}

void Widget::on_tableResultats_itemClicked(QTableWidgetItem *item)
{
    QString filename = ui->tableResultats->item(item->row(), 1)->text();
    if(filename.toLower().endsWith(".png")) {
        apercuImage(filename);
    } else {
        apercuTexte(filename);
    }
}
void Widget::apercuTexte(QString filename) {
    ui->apercu->setCurrentIndex(0);
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream s1(&file);

    QTextCodec *codec = QTextCodec::codecForMib(106);
    s1.setAutoDetectUnicode(true);
    s1.setCodec(codec);

    textApercu->setPlainText(s1.readAll());
    file.close();

    if (highlighter){
        delete highlighter;
        highlighter = 0;
    }
    if(textApercu->blockCount() > 10000) {
        return;
    }
    if(filename.toLower().endsWith(".png")) {
    } else if(filename.toLower().endsWith(".java")) {
        highlighter = new JavaHighlighter(textApercu->document(), regularExpression());
    } else if(filename.toLower().endsWith(".xml")) {
        highlighter = new XmlHighlighter(textApercu->document(), regularExpression());
    } else if(filename.toLower().endsWith(".sql")) {
        highlighter = new SqlHighlighter(textApercu->document(), regularExpression());
    } else {
        highlighter = new Highlighter(textApercu->document(), regularExpression());
    }
    //surligne();

}
void Widget::apercuImage(QString filename) {
    ui->apercu->setCurrentIndex(1);
    QLabel *label = (QLabel*) ui->apercu->widget(1);
    QPixmap pixmap;
    pixmap.load(filename);
    pixmap = pixmap.scaledToHeight(qMin(label->size().height(), pixmap.height()), Qt::SmoothTransformation);
    label->setPixmap(pixmap);
}

void Widget::on_precedent_clicked()
{
    QString text = ui->comboContenant->currentText();
    if (text.isEmpty()) {
        return;
    }
    textApercu->find(regularExpression(), QTextDocument::FindBackward);
    QTextCursor cursor = textApercu->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    textApercu->setTextCursor(cursor);
    textApercu->ensureCursorVisible();
}

void Widget::on_suivant_clicked()
{
    QString text = ui->comboContenant->currentText();
    if (text.isEmpty()) {
        return;
    }
    textApercu->find(regularExpression());
    QTextCursor cursor = textApercu->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    textApercu->setTextCursor(cursor);
    textApercu->ensureCursorVisible();

}

void Widget::on_premier_clicked()
{
    QTextCursor cursor = textApercu->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    textApercu->setTextCursor(cursor);
    on_suivant_clicked();
}

void Widget::on_dernier_clicked()
{
    //textApercu->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    QTextCursor cursor = textApercu->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    textApercu->setTextCursor(cursor);
    on_precedent_clicked();
}

void Widget::on_tableResultats_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableResultats->itemAt(pos);
    if (item) {
        qDebug() << item->text();
        QPoint globalPos = qobject_cast< QWidget* >( sender() )->mapToGlobal( pos );
        QMenu *menu = new QMenu(ui->tableResultats);
        QAction *copy = menu->addAction(QIcon(":/images/bell.png"), tr("Copier le chemin"));
        copy->setData(QDir::toNativeSeparators(item->text()));
        connect(copy, SIGNAL(triggered()), this, SLOT(copierTexte()));
        menu->exec(globalPos);
        delete menu;
    }
}

void Widget::copierTexte() {
    QAction *emetteur = (QAction *) sender();
    QString texte = emetteur->data().toString();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(texte);
}

void Widget::on_notepadd_clicked()
{
    QTableWidgetItem *item = ui->tableResultats->item(ui->tableResultats->currentRow(), 1);
    qDebug() << item << ui->tableResultats->currentRow();
    if(item == 0) {
        return;
    }
    if( !QProcess::startDetached("notepad++", QStringList() << QDir::toNativeSeparators(item->text()))) {
        QMessageBox::information(
            this,
            tr("QChercheur"),
            tr("notepad++ doit être dans le PATH") );
    }
}

void Widget::slotFind(Ui::FindWidget uiFind, QString ttf, QTextDocument::FindFlags options, bool fromButton)
{
    QTextDocument *doc = textApercu->document();
    QString oldText = uiFind.editFind->text();
    QTextCursor c = textApercu->textCursor();
    QPalette p = uiFind.editFind->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::white);

    if (c.hasSelection())
    {
        if ( options & QTextDocument::FindBackward || fromButton)
            c.setPosition(c.position(), QTextCursor::MoveAnchor);
        else
            c.setPosition(c.anchor(), QTextCursor::MoveAnchor);
    }


    QTextCursor newCursor = c;

    if (!ttf.isEmpty())
    {
        newCursor = doc->find(ttf, c, options);
        uiFind.labelWrapped->hide();

        if (newCursor.isNull())
        {
            QTextCursor ac(doc);
            ac.movePosition(options & QTextDocument::FindBackward
                            ? QTextCursor::End : QTextCursor::Start);
            newCursor = doc->find(ttf, ac, options);
            if (newCursor.isNull())
            {
                p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
                newCursor = c;
            }
            else
            {
                uiFind.labelWrapped->show();
                //uiFind.reached->setText(tr("Reached end of page, continued from top"));
                uiFind.labelWrapped->show();
            }
        } else {
            newCursor.blockCharFormat().setBackground(( QBrush( Qt::green )));
        }
    }
    textApercu->setTextCursor(newCursor);
    uiFind.editFind->setPalette(p);

}

void Widget::find()
{
    this->finder->find();
}

QString Widget::repertoire()
{
    return ui->comboRepertoires->currentText();
}

void Widget::findPrevious()
{
    this->finder->findPrevious();
}

void Widget::findNext()
{
    this->finder->findNext();
}


void Widget::nouvelleRecherche(QString selectedText) {
    qDebug() << "parent" << parentWidget() << qApp->activeWindow();
    MainWindow* mainwindow = qobject_cast<MainWindow *>(qApp->activeWindow());
    if(mainwindow == 0) {
        qDebug() << "mainwindow 0" << parentWidget();
    }
    //MainWindow* main = (MainWindow *) parentWidget();
    QTableWidgetItem *item = ui->tableResultats->item(ui->tableResultats->currentRow(), 1);
    if(item == 0 || selectedText == "") {
        return;
    }
    QString chemin = QFileInfo(item->text()).absoluteDir().absolutePath();
    mainwindow->nouvelleRecherche(chemin, selectedText);
}



void Widget::on_comboContenant_editTextChanged(const QString &texte)
{
    MainWindow* mainwindow = qobject_cast<MainWindow *>(qApp->activeWindow());
    mainwindow->changeTitre(this, texte);
}

void Widget::on_comboRepertoires_editTextChanged(const QString &texte)
{
    QLineEdit *text = ui->comboRepertoires->lineEdit();
    QPalette palette = text->palette();
    QString chemin = QDir::toNativeSeparators(texte);
    if(QDir(chemin).exists()) {
        palette.setColor(QPalette::Text, QColor(0,0,0));
    } else {
        palette.setColor(QPalette::Text, QColor(255,0,0));

    }
    text->setPalette(palette);
}
