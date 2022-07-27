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

void Translator::Tranlate_front()
{
    QLabel *waiting = new QLabel(mainwindow);
    waiting->setGeometry(mainwindow->width()/2-50,mainwindow->height()/2-100,100,100);
    waiting->setAlignment(Qt::AlignCenter);
    waiting->setStyleSheet("background:transparent");
    QMovie *m_move = new QMovie(":/Loading.gif");
    m_move->setScaledSize(QSize(50,50));
    waiting->setMovie(m_move);


    QString SourceText=ui->Source_Edit->toPlainText(); //获取待翻译的文本
    QString TargetText;

    QString intype=ui->label->text()=="英语"?"en":"zh_CN";
    QString outtype=ui->label_2->text()=="英语"?"en":"zh_CN";

    m_move->start();
    waiting->show();


    TranslateByGoogle(SourceText,TargetText,intype,outtype);

    m_move->stop();
    waiting->hide();
    ui->Target_Edit->setText(TargetText);
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
            QMessageBox::warning(mainwindow,"网络连接异常","请求失败，请检查网络连接！", QMessageBox::Ok, QMessageBox::Ok);
            qDebug()<<"GoogleAPI::postHttp() QNetworkReply Error String : " << pReply->errorString();
            return -1;
        }
        else
        {
            QByteArray data = pReply->readAll();

            //如果有错误的序列号
            int errorIndex = data.indexOf("error_code");
            if (errorIndex != -1)
                return errorIndex;

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
            return 0;
        }
    }
    else
    {  // 处理超时
        disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        pReply->abort();
        pReply->deleteLater();
        QMessageBox::warning(mainwindow,"请求超时","请求超时，请检查网络连接！", QMessageBox::Ok, QMessageBox::Ok);
        qDebug()<<"QNetworkReply Timeout";
        return -1;
    }
    return 0;
}
