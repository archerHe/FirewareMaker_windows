#include "texthelper.h"


TextHelper::TextHelper()
{
    curPath = QDir::currentPath();

}

bool TextHelper::modifyTextStr(QString filePath, QString oriStr, QString newStr)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadWrite))
    {

        QTextStream stream(&file);
        oriText = stream.readAll();
        newText = oriText.replace(oriStr, newStr);
    }
    else
    {
        qDebug() << "modifyTextStr: " << " open file fail";
        return false;
    }
    return true;
}

QString TextHelper::readTextStr(QString filePath, QString objStr, QString typeFlag)
{
    QString resultStr;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString objLine;
        while(!stream.atEnd())
        {
            objLine = stream.readLine();
            if(objLine.contains(objStr))
            {
                if(typeFlag == "xml")
                {
                    resultStr = readXml(objLine);
                    qDebug() <<  "objStr:" << objStr << "result:" << resultStr;
                    break;
                }else if(typeFlag == "boardCfg")
                {
                    QStringList strlist = objLine.split(":=");
                    resultStr = strlist[1];
                    break;
                }else if(typeFlag == "kernelCfg")
                {
                    return "IPS";
                }
                QStringList strlist = objLine.split("=");
                resultStr = strlist[1];
                break;
            }
        }
    }
    file.close();
    qDebug() << objStr << " : " << resultStr;
    return resultStr.trimmed();
}

QString TextHelper::readXml(QString xmlLine)
{
    QStringList strlist1 = xmlLine.split("\">");
    QString str = strlist1[1];
    QStringList strlis2 = str.split("<");
    QString resultStr = strlis2[0];
    return resultStr.trimmed();
}

bool TextHelper::modifyXml(QString filePath, QString attr, QString newStr)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        return false;
    }
    file.close();

    QDomNode n = doc.firstChild();
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            QDomNodeList nodeList = e.childNodes();
            for(int i = 0; i < nodeList.count(); i++)
            {
                QDomNode node = nodeList.at(i);
                if(node.isElement())
                {
                    if(node.toElement().attribute("name") == attr)
                    {
                        node.toElement().firstChild().setNodeValue(newStr);
                    }
                }
            }
        }
       n = n.nextSibling();
    }

    QFile temp(Global::prj_home_path + "/tmp/tmp.xml");
    if(!temp.open(QIODevice::WriteOnly))
    {
        qDebug() << "tmp/tmp.xml open fail!!";
        return false;
    }

    QTextStream out(&temp);
    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    temp.flush();
    temp.close();
    QFile::remove(filePath);
    QFile::copy(Global::prj_home_path + "/tmp/tmp.xml", filePath);

}

int TextHelper::readCam(QString camType, QString dtsPath)
{
    QFile   *dtsFile =  new QFile(dtsPath);
    int camera_id = 0;
    QTextStream ts(dtsFile);
    if(!dtsFile->open(QIODevice::ReadOnly))
        return -1;
    if(camType == "back")
    {
        QString strLine;
        while(!ts.atEnd())
        {
            strLine = ts.readLine();
            QString s = "camera" + QString::number(camera_id, 10);
            if(strLine.contains(s) && strLine.contains(":"))
            {
                strLine = ts.readLine();
                if(strLine.contains("//"))
                {
                    strLine = ts.readLine();
                    QStringList strlist1 = strLine.split(",");
                    QString str2 = strlist1[1];
                    QStringList strlist2 = str2.split("-");
                    qDebug() << strlist2[0] << " camera_id: " << camera_id;
                    return camera_id;
                }
                camera_id++;
            }
        }
    }
    if(camType == "front")
    {
        camera_id = 6;
        QString strLine;
        while(!ts.atEnd())
        {
            strLine = ts.readLine();
            QString s = "camera" + QString::number(camera_id, 10);
            if(strLine.contains(s) && strLine.contains(":"))
            {

                strLine = ts.readLine();
                qDebug() << strLine;

                if(strLine.contains("compatible") || strLine.contains("//"))
                {
                    if(strLine.contains("//"))
                    {
                        strLine = ts.readLine();
                    }

                    QStringList strlist1 = strLine.split(",");
                    QString str2 = strlist1[1];
                    QStringList strlist2 = str2.split("-");
                    qDebug() << strlist2[0] << "camera_id: " << camera_id;
                    return camera_id - 6;
                }
                camera_id++;
            }

        }

    }
    dtsFile->close();
}

void TextHelper::writeCam(int preCamId, int curCamId, QString dtsPath)
{
    if(preCamId == curCamId)
        return;
    disableCam(preCamId, dtsPath);
    //enableCam(curCamId, dtsPath);
}

int TextHelper::disableCam(int camId, QString dtsPath)
{
    //QString dtsPath;


    QFile   *dtsFile =  new QFile(dtsPath);
    QFile   *newDtsFile = new QFile(QDir::currentPath() + "/tmp/Sofia3GR-tablet.dts");
    QTextStream newDts(newDtsFile);
    QTextStream dtsOld(dtsFile);
    QString strLine;
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        return -1;
    }
    newDtsFile->open(QIODevice::WriteOnly);
    while(!dtsOld.atEnd())
    {
        strLine = dtsOld.readLine();
        if(strLine.contains("camera" + QString::number(camId, 10)) && strLine.contains(":"))
        {
               newDts << strLine << "\n";
               newDts << "\t\tstatus = \"disabled\";\n";
               if(camId == 7)
               {
                   newDts << "\t\tcompatible = \"galaxycore,gc0310-v4l2-i2c-subdev\";\n";
               }
               strLine = dtsOld.readLine();
               continue;
        }
        newDts << strLine << "\n";

    }
    dtsFile->close();
    newDtsFile->flush();
    newDtsFile->close();
   // dtsFile->remove();

    if(dtsFile->remove())
    {
        qDebug() << "remove ok disableCam";
    }else
    {
        qDebug() << "remove fail disableCam";
        qDebug() << "(disableCam)dtsPath: " << dtsPath;
        QStringList strList;
        strList << "Z:/heyuan/android_rockchip_sofia3gr_5_1_source_code_2015_09_13_20_35_28/kernel/arch/x86/boot/dts/SF_3GR-tablet.dts";
        QProcess::startDetached("del", strList);
    }
    if(newDtsFile->copy(dtsPath))
    {
        qDebug() << "camId:" << camId << " file copy ok (disableCam)";
    }else
    {
        qDebug() << "camId: " << camId << "file copy fail!!! (disableCam)";
    }

    return camId;
}

int TextHelper::enableCam(int camId, QString dtsPath)
{
    //QString dtsPath;
    QFile   *dtsFile =  new QFile(dtsPath);
    QFile   *newDtsFile = new QFile(QDir::currentPath() + "/tmp/Sofia3GR-tablet.dts");
    QTextStream newDts(newDtsFile);
    QTextStream dtsOld(dtsFile);
    QString strLine;
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        return -1;
    }
    newDtsFile->open(QIODevice::WriteOnly);
    while(!dtsOld.atEnd())
    {
        strLine = dtsOld.readLine();
        if(strLine.contains("camera" + QString::number(camId, 10)) && strLine.contains(":"))
        {
               newDts << strLine << "\n";
               newDts << "\t\t//status = \"disabled\";\n";
               strLine = dtsOld.readLine();
               continue;
        }
        newDts << strLine << "\n";

    }
    dtsFile->close();
    newDtsFile->flush();
    newDtsFile->close();
    dtsFile->remove();
    if(dtsFile->remove())
    {
        qDebug() << "remove ok enableCam";
    }else
    {
        qDebug() << "remove fail enableCam";
        qDebug() << "(enableCam)dtsPath: " << dtsPath;
        QFile file("Z:/heyuan/android_rockchip_sofia3gr_5_1_source_code_2015_09_13_20_35_28/kernel/arch/x86/boot/dts/SF_3GR-tablet.dts");
        if(file.remove())
        {
            qDebug() << "file.remove~~~~~~";
        }else
        {
            qDebug() << "file.remove fail .....";
        }

    }

    if(newDtsFile->copy(dtsPath))
    {
        qDebug() << "camId:" << camId << " file copy ok (enableCam)";
    }else
    {
        qDebug() << "camId: " << camId << "file copy fail!!!(enableCam)";
        return -1;
    }

    return camId;
}

bool TextHelper::writeToText(QString filePath, QString str, QString value, QString split)
{
    QFile *oriFile = new QFile(filePath);
    QFile *tempFile = new QFile(QDir::currentPath() + "/tmp/temp.txt");
    QTextStream oriTS(oriFile);
    QTextStream tempTS(tempFile);
    QString strLine;
    if(!oriFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "oriFile return false";
        return false;
    }
    if(!tempFile->open(QIODevice::WriteOnly))
    {
        qDebug() << "tempFile return false";
        return false;
    }
    while(!oriTS.atEnd())
    {
        strLine = oriTS.readLine();
        if(strLine.contains(str))
        {
            if(split == "id")
            {
                tempTS << "export " << str << " := " << value << "\n";
                continue;
            }
            tempTS << str << split << value << "\n";
            qDebug() << "tempTS: " << str << split << value;
            continue;
        }
        tempTS << strLine << "\n";
    }
    oriFile->close();
    tempFile->flush();
    tempFile->close();
    oriFile->remove();
    if(tempFile->copy(filePath))
    {
        qDebug() << "copy ok" << filePath;
    }else
    {
        qDebug() << "copy fail" << filePath << "  str: " << str;
        return false;
    }
    return true;
}

bool TextHelper::addWallpaperXml(QString filePath, QString newStr)
{
    QFile       wallpaperXml(filePath);
    QFile       tempXml(QDir::currentPath() + "/tmp/temp.xml");
    qDebug() << "tempxml: " << QDir::currentPath() + "/tmp/temp.xml";
    QTextStream wallpaperTS(&wallpaperXml);
    QTextStream tempTS(&tempXml);
    QString     strLine;
    if(!wallpaperXml.open(QIODevice::ReadOnly))
    {
        qDebug() << "wallpaperXml open fail";
        return false;
    }
    if(!tempXml.open(QIODevice::WriteOnly))
    {
        qDebug() << "tempXml open fail";
        return false;
    }
    while(!wallpaperTS.atEnd())
    {
        strLine = wallpaperTS.readLine();
        if(strLine.contains("<item>default_wallpaper"))
        {
            tempTS << strLine << "\n";
            tempTS << "\t\t<item>" << newStr << "</item>" << "\n";
            continue;
        }
        tempTS << strLine << "\n";
    }
    wallpaperXml.close();
    tempXml.flush();
    tempXml.close();
    wallpaperXml.remove();

    if(tempXml.copy(filePath))
    {
        qDebug() << "str:" << newStr << " file copy ok";
    }else
    {
        qDebug() << "addWallpaperXml: str: " << newStr << "file copy fail!!!";
    }
}



