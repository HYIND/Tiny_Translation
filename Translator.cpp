#include "Translator.h"
#include <QNetworkRequest>
#include <QTimer>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QLabel>
#include <QMovie>
#include <QKeyEvent>
#include <QEvent>
#include <windows.h>
#include <QWaitcondition>

Translator tran;

void Translator::Tranlate_front()
{
    QLabel *waiting = new QLabel(pmainwindow);
    waiting->setGeometry(pmainwindow->width()/2-50,pmainwindow->height()/2-100,100,100);
    waiting->setAlignment(Qt::AlignCenter);
    waiting->setStyleSheet("background:transparent");
    QMovie *m_move = new QMovie(":/Loading.gif");
    m_move->setScaledSize(QSize(50,50));
    waiting->setMovie(m_move);


    QString SourceText=main_ui->Source_Edit->toPlainText(); //获取待翻译的文本
    QString TargetText;

    QString intype=main_ui->label->text()=="英语"?"en":"zh_CN";
    QString outtype=main_ui->label_2->text()=="英语"?"en":"zh_CN";

    m_move->start();
    waiting->show();

    if(!TranslateByGoogle(SourceText,TargetText,intype,outtype))
    {
        QMessageBox::warning(pmainwindow,"请求异常","请求失败，请检查网络连接！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_move->stop();
    waiting->hide();
    main_ui->Target_Edit->setText(TargetText);
}
bool Translator::TranslateByGoogle(QString& in,QString& out,QString intype,QString outtype)
{
    QNetworkRequest request;
    //    request.setUrl(QUrl("http://translate.google.cn/translate_a/single?client=gtx&sl=en&tl=zh-CN&dt=t&q="+SourceText));
    request.setUrl(QUrl("http://translate.google.com/translate_a/single?client=gtx&dt=t&dj=1&ie=UTF-8&sl="+intype+"&tl="+outtype+"&q="+in));

    //    QVariant userAgent=
    //    request.setHeader(QNetworkRequest::UserAgentHeader, m_userAgent);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    QTimer timer;
    timer.setInterval(5000);  // 设置超时时间 5 秒
    timer.setSingleShot(true);  // 单次触发
    QNetworkAccessManager networkManager;
    QNetworkReply *pReply = networkManager.get(request);
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start();

    loop.exec();
    if (timer.isActive())
    {
        // 处理响应
        timer.stop();
        if (pReply->error() != QNetworkReply::NoError)
        {
            // 错误处理
            qDebug()<<"GoogleAPI::postHttp() QNetworkReply Error String : " << pReply->errorString();
            return false;
        }
        else
        {
            QByteArray data = pReply->readAll();

            //如果有错误的序列号
            int errorIndex = data.indexOf("error_code");
            if (errorIndex != -1)
                return false;

            //json解析
            QJsonParseError jsonError;
            QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
            if(!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
            {
                if (doucment.isObject())
                {
                    QJsonObject jsonObject=doucment.object();
                    if(jsonObject.contains("sentences"))
                    {
                        QJsonValue sen_jsonValue=jsonObject.value("sentences");
                        if(sen_jsonValue.isArray())
                        {
                            QJsonArray Array_json=sen_jsonValue.toArray();
                            for(int i=0;i<Array_json.count();i++)
                            {
                                QJsonValue Array_jsonValue=Array_json.at(i);
                                if(Array_jsonValue.isObject())
                                {
                                    QJsonObject Array_jsonObject = Array_jsonValue.toObject();
                                    if(Array_jsonObject.contains("trans"))
                                    {
                                        QJsonValue trans_jsonValue =Array_jsonObject.value("trans");
                                        if(trans_jsonValue.isString())
                                        {
                                            out+=trans_jsonValue.toString();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }
    }
    else
    {  // 处理超时
        disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        pReply->abort();
        pReply->deleteLater();
        qDebug()<<"QNetworkReply Timeout";
        return false;
    }
    return true;
}

void Translator::Tranlate_back_emit()
{
    emit back_tran();
}

void Translator::Tranlate_back()
{
    //复制选中内容到剪切板
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QString last=clipboard->text();

    QMimeData *clipData = new QMimeData();
        for(int i = 0; i < mimeData->formats().size(); i++)       //保存剪切板中数据
        {
            QString type = mimeData->formats().at(i);        //保存剪切板中数据格式
            QByteArray data = mimeData->data(type);       //按照数据格式保存数据
            clipData->setData(type,data);
        }

    keybd_event(VK_CONTROL,0,0,0);
    keybd_event('C',0,0,0);
    keybd_event('C',0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
    _sleep(100);

    QString source=clipboard->text();
    if(source.isEmpty()||source==last)
    {
        delete clipData;
        return;
    }
     qDebug()<<source;
    QString out;
    TranslateByGoogle(source,out);
    emit back_tran_finish(out,clipData);

//    if (mimeData->hasImage()) {
//        return;
//    }
//    else if(mimeData->hasUrls()){
//        return;
//    }
//    else if (mimeData->hasText()) {
//        QString source=mimeData->text();
//        if(source!="")
//        {
////            if(source==last)
////                return;
//                    qDebug()<<source;
////            last =source;
//            QString out;
//            TranslateByGoogle(source,out);
//            emit back_tran_finish(out,clipData);
//        }
//    }
}
