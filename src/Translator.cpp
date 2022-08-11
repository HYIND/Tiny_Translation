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
#include <QTextCodec>
#include <QRandomGenerator>
#include <string>
#include <QCryptographicHash>

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

    m_move->start();
    waiting->show();

    switch(main_ui->API_ComboBox->currentIndex())
    {
    case 0:
    {
        QString intype=main_ui->label->text()=="英语"?"en":"zh-CN";
        QString outtype=main_ui->label_2->text()=="英语"?"en":"zh-CN";

        if(!TranslateByGoogle(SourceText,TargetText,intype,outtype))
        {
            QMessageBox::warning(pmainwindow,"请求异常","请求失败，请检查网络连接！", QMessageBox::Ok, QMessageBox::Ok);
        }
        break;
    }
    case 1:
    {
        QString intype=main_ui->label->text()=="英语"?"en":"zh-CHS";
        QString outtype=main_ui->label_2->text()=="英语"?"en":"zh-CHS";

        if(!TranslateByYoudao(SourceText,TargetText,intype,outtype))
        {
            QMessageBox::warning(pmainwindow,"请求异常","请求失败，请检查网络连接！", QMessageBox::Ok, QMessageBox::Ok);
        }
        break;
    }
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

bool Translator::TranslateByYoudao(QString& in,QString& out,QString intype,QString outtype)
{
    QNetworkRequest request;
    if(intype=="AUTO")
        request.setUrl(QUrl("http://fanyi.youdao.com/translate?&doctype=json&i="+in));
    request.setUrl(QUrl("http://fanyi.youdao.com/translate?&doctype=json&type="+intype+"2"+outtype+"&i="+in));
    //    request.setUrl(QUrl("http://fanyi.youdao.com/translate?smartresult=dict&smartresult=rule"));

    //    QDateTime time = QDateTime::currentDateTime();
    //    qint64 t= time.toMSecsSinceEpoch();     //时间戳

    //    request.setRawHeader("Cookie","OUTFOX_SEARCH_USER_ID=-90618124@10.110.96.158; JSESSIONID=abc02ZkRA6Zp7_uFgjDhy; OUTFOX_SEARCH_USER_ID_NCOO=736067356.8751284; YOUDAO_FANYI_SELECTOR=OFF; SESSION_FROM_COOKIE=test; ___rl__test__cookies="
    //                         +QString::number(t-1).toUtf8());
    //    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    //    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.5112.81 Safari/537.36 Edg/104.0.1293.47");
    ////    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    //    request.setRawHeader("Accept-Encoding" ,"gzip, deflate, br");
    //    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6");
    //    request.setRawHeader("Connection", "keep-alive");
    //    request.setRawHeader("Content-Length", "337");
    //            request.setRawHeader("Content-Type","application/x-www-form-urlencoded; charset=UTF-8");
    //    request.setRawHeader("DNT", "1");
    //    request.setRawHeader("Host", "fanyi.youdao.com");
    //    request.setRawHeader("Origin", "http://fanyi.youdao.com");
    //    request.setRawHeader("Referer", "http://fanyi.youdao.com/");
    //    request.setRawHeader("sec-ch-ua", "\"Chromium\";v=\"104\", \" Not A;Brand\";v=\"99\", \"Microsoft Edge\";v=\"104\"");
    //    request.setRawHeader("sec-ch-ua-mobile", "?0");
    //    request.setRawHeader("sec-ch-ua-platform", "\"Windows\"");
    //    request.setRawHeader("Sec-Fetch-Dest", "empty" );
    //    request.setRawHeader("Sec-Fetch-Mode","cors");
    //    request.setRawHeader("Sec-Fetch-Site", "same-origin");
    //    request.setRawHeader("X-Requested-With", "XMLHttpRequest");


    //    QRandomGenerator* rand=new QRandomGenerator();
    //    std::string lts= QString::number(t).toStdString();  //时间戳
    //    std::string salt = lts + QString::number(rand->bounded(0,10)).toStdString(); //当前时间戳+随机数
    //    QString s="fanyideskweb"+in+QString::fromStdString(salt)+"Ygy_4c=r#e#4EX^NUGUc5";

    //计算sign 的 MD5值
    //    std::string youdao_sign = QCryptographicHash::hash(s.toLatin1(),QCryptographicHash::Md5).toHex().toStdString();

    //    std::string data;
    //    data.clear();
    //    data.append("i=");data.append(in.toStdString());
    //    data.append("&from=");data.append(intype.toStdString());data.append("&to=");data.append(outtype.toStdString());
    //    data.append("&smartresult=dict&smartresult=rule&client=fanyideskweb&salt=");data.append(salt);
    //    data.append("&sign=");data.append(youdao_sign);
    //    data.append("&lts=");data.append(lts);
    //    data.append("&bv=3e20416a29e1d1549139291c2bf51e5a&doctype=json&version=2.1&keyfrom=fanyi.web&action=FY_BY_CLICKBUTTION");


    QTimer timer;
    timer.setInterval(5000);  // 设置超时时间 5 秒
    timer.setSingleShot(true);  // 单次触发


    QNetworkAccessManager networkManager;

    QNetworkReply *pReply = networkManager.get(request);
    //    QNetworkReply *pReply=networkManager.post(request, QString::fromStdString(data).toUtf8());

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
            qDebug()<<"YoudaoAPI::postHttp() QNetworkReply Error String : " << pReply->errorString();
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
                    if(jsonObject.contains("translateResult"))
                    {
                        QJsonValue translateResult_jsonValue=jsonObject.value("translateResult");
                        if(translateResult_jsonValue.isArray())
                        {
                            QJsonArray translateResult_Array1_json=translateResult_jsonValue.toArray();
                            QJsonValue translateResult_Array2_jsonValue=translateResult_Array1_json.at(0);
                            if(translateResult_Array2_jsonValue.isArray())
                            {
                                QJsonArray translateResult_Array2_json = translateResult_Array2_jsonValue.toArray();
                                QJsonValue translateResult_Object_jsonValue=translateResult_Array2_json.at(0);
                                if(translateResult_Object_jsonValue.isObject())
                                {
                                    QJsonObject translateResult_Object_json =translateResult_Object_jsonValue.toObject();
                                    if(translateResult_Object_json.contains("tgt"))
                                    {
                                        QJsonValue translateResult_jsonValue =translateResult_Object_json.value("tgt");
                                        if(translateResult_jsonValue.isString())
                                        {
                                            out=translateResult_jsonValue.toString();
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return false;
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

    switch(main_ui->API_ComboBox->currentIndex())
    {
    case 0:
    {

        TranslateByGoogle(source,out);
        break;
    }
    case 1:
    {
        TranslateByYoudao(source,out);
        break;
    }
    }
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
