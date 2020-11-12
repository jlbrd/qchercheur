#include "widget.h"
#include "ui_widget.h"
#include "javahighlighter.h"
#include "xmlhighlighter.h"
#include "mainwindow.h"
#include "find.h"
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


Widget::Widget(QWidget *parent, QString chemin, QString texte) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    textApercu = (TextEdit*) ui->apercu->widget(0);
    finder = new Find(ui->findWidgetContainer, this, textApercu);
    ui->tableResultats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWindowFlags(Qt::Widget);
    ui->info->setText("");
    ui->nbOccurrences->setText("");
    if(chemin != "") {
        ui->comboRepertoires->setCurrentText(chemin);
    }
    if(texte != "") {
        ui->comboContenant->setCurrentText(texte);
    }
    connect(textApercu, SIGNAL(nouvelleRecherche(QString)), this, SLOT(nouvelleRecherche(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_boutonDepart_clicked()
{
    stopper = false;
    ui->boutonDepart->setEnabled(false);
    ui->tableResultats->setRowCount(0);
    ui->tableResultats->clearContents();
    textApercu->setPlainText("");
    ui->nbOccurrences->setText("");
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
        QString nomFichier = it->next();
        //qDebug() << nomFichier;
        ui->info->setText(QFileInfo(nomFichier).absoluteDir().absolutePath());
        if(repertoireExclu(nomFichier)) {
            continue;
        }
        else if(ui->comboContenant->currentText() == "" || fichierContient(nomFichier)) {
            ui->tableResultats->insertRow(ui->tableResultats->rowCount());
            ui->tableResultats->setRowCount(ui->tableResultats->rowCount());
            int numLigne = ui->tableResultats->rowCount()-1;
            ui->tableResultats->setItem(numLigne, 0, new QTableWidgetItem(nomFichier.section("/", -1)));
            ui->tableResultats->setItem(numLigne, 1, new QTableWidgetItem(nomFichier));
            ui->tableResultats->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        }
        QApplication::processEvents(QEventLoop::AllEvents);
    }
    delete it;
    ui->boutonDepart->setEnabled(true);
    ui->info->setText("");
    stopper = false;
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

bool Widget::fichierContient(QString nomFichier) {
    QFile fichier(nomFichier);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in (&fichier);
    QString line;
    do {
        line = in.readLine();
        if (line.contains(ui->comboContenant->currentText(), Qt::CaseInsensitive)) {
            fichier.close();
            return true;
        }
    } while (!line.isNull());
    fichier.close();
    return false;
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
    if(filename.toLower().endsWith(".png")) {
    } else if(filename.toLower().endsWith(".java")) {
        highlighter = new JavaHighlighter(textApercu->document());
    } else if(filename.toLower().endsWith(".xml")) {
        highlighter = new XmlHighlighter(textApercu->document());
    }
    else if (highlighter){
        delete highlighter;
        highlighter = 0;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream s1(&file);

    QTextCodec *codec = QTextCodec::codecForMib(106);
    s1.setAutoDetectUnicode(true);
    s1.setCodec(codec);

    textApercu->setPlainText(s1.readAll());
    file.close();
    surligne();

}
void Widget::apercuImage(QString filename) {
    ui->apercu->setCurrentIndex(1);
    QLabel *label = (QLabel*) ui->apercu->widget(1);
    QPixmap pixmap;
    pixmap.load(filename);
    pixmap = pixmap.scaledToHeight(qMin(label->size().height(), pixmap.height()), Qt::SmoothTransformation);
    label->setPixmap(pixmap);
}

void Widget::surligne()
{
    QString text = ui->comboContenant->currentText();
    if (text.isEmpty()) {
        return;
    }
    QList<QTextEdit::ExtraSelection> extraSelectionsList;
    textApercu->setExtraSelections( QList<QTextEdit::ExtraSelection>() );
    int nbOccurrences = 0;
    while ( textApercu->find(text) )
    {
        QTextEdit::ExtraSelection highlight;
        highlight.cursor = textApercu->textCursor();
        highlight.format.setBackground( QBrush( Qt::yellow ) );
        extraSelectionsList << highlight;
        nbOccurrences++;
    }
    textApercu->setExtraSelections( extraSelectionsList );
    textApercu->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

    QTextCursor newCursor = textApercu->document()->find(text);
    newCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    //highlight.cursor.movePosition( QTextCursor::PreviousWord );
    //highlight.cursor.movePosition( QTextCursor::EndOfWord, QTextCursor::KeepAnchor );

    textApercu->setTextCursor(newCursor);
    textApercu->ensureCursorVisible();
    ui->nbOccurrences->setText(QString::number(nbOccurrences) + " résultat" + (nbOccurrences > 1 ? "s" : ""));

}

void Widget::on_premier_clicked()
{
    textApercu->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    on_suivant_clicked();
}

void Widget::on_precedent_clicked()
{
    QString text = ui->comboContenant->currentText();
    if (text.isEmpty()) {
        return;
    }
    textApercu->find(text, QTextDocument::FindBackward);
    textApercu->ensureCursorVisible();
}

void Widget::on_suivant_clicked()
{
    QString text = ui->comboContenant->currentText();
    if (text.isEmpty()) {
        return;
    }
    textApercu->find(text);
    textApercu->ensureCursorVisible();

}

void Widget::on_dernier_clicked()
{
    //
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


