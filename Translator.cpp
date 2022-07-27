#include "Translator.h"
#include <QNetworkRequest>
#include <QTimer>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

Ui::MainWindow *ui = nullptr;

void ui_init(Ui::MainWindow *ui_ptr)
{
    ui = ui_ptr;
}

bool Translator::TranslateByGoogle()
{
    QString SourceText=ui->Source_Edit->toPlainText(); //获取待翻译的文本
    QNetworkRequest request;
    request.setUrl(QUrl("http://translate.google.cn/translate_a/single?client=gtx&sl=en&tl=zh-CN&dt=t&q="+SourceText));
    //http://translate.google.com/translate_a/single?client=gtx&dt=t&dj=1&ie=UTF-8&sl=auto&tl=zh_CN&q=test

    //    QVariant userAgent=
    //    request.setHeader(QNetworkRequest::UserAgentHeader, m_userAgent);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    QTimer timer;
    timer.setInterval(10000);  // 设置超时时间 10 秒
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
            QString tran_result;
            if(!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
            {
                if (doucment.isArray())
                {
                    QJsonArray array = doucment.array();
                    if(array.count() > 0)
                    {
                        QJsonValue value = array.at(0);
                        if(value.isArray())
                        {
                            QJsonArray array1 =  value.toArray();
                            if(array1.count() > 0)
                            {
                                for(int i=0;i<array1.count();i++)
                                {
                                    QJsonValue value1 = array1.at(i);
                                    if(value1.isArray())
                                    {
                                        QJsonArray array2 =  value1.toArray();
                                        if(array2.count() > 0)
                                        {
                                            QString p_translate = array2.at(0).toString();
                                            tran_result+=p_translate;
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
            ui->Target_Edit->setText(tran_result);
            return 0;
        }
    }
        else
        {  // 处理超时
            disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            pReply->abort();
            pReply->deleteLater();
            qDebug()<<"QNetworkReply Timeout";
            return -1;
        }
    return 0;
}
