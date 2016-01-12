#include "texthelper.h"


TextHelper::TextHelper()
{

}
/*
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
*/
QString TextHelper::readTextStr(QString filePath, QString objStr, QString typeFlag)
{
    QString resultStr = NULL;
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
                    break;
                }else if(typeFlag == "boardCfg")
                {
                    if(objStr == "BAND_FEID")
                    {
                        if(objLine.at(0) == '#')
                            continue;
                    }
                    QStringList strlist = objLine.split(":=");
                    resultStr = strlist[1];
                    break;
                }else if(typeFlag == "kernelCfg")
                {
                    file.close();
                    return "IPS";
                }else if(typeFlag == "flash")
                {
                    objLine = stream.readLine();
                    objLine = stream.readLine();
                    objLine = stream.readLine();
                    objLine = stream.readLine();
                    objLine = stream.readLine();
                    objLine = stream.readLine();
                }
                QStringList strlist = objLine.split("=");
                resultStr = strlist[1];
                break;
            }
        }
    }
    file.close();
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
        Global::saveErr.append(filePath + "open fail\n");
        return false;
    }
    QDomDocument doc;

    if(!doc.setContent(&file))
    {
        file.close();
        Global::saveErr.append("QDomDocument setContent fail");
        return false;
    }
    file.close();
  //  QDomProcessingInstruction
    QDomNode n = doc.firstChild();
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            QDomNodeList nodeList = e.childNodes();
            qDebug() << "nodeList length : " << nodeList.count();
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
        Global::saveErr.append(Global::prj_home_path + "/tmp/tmp.xml open fail\n");
        return false;
    }

    QTextStream out(&temp);
  //  out.setCodec("UTF-8");
    doc.save(out, 4, QDomNode::EncodingFromDocument);
    temp.flush();
    temp.close();
    QFile::remove(filePath);
    if(!QFile::copy(Global::prj_home_path + "/tmp/tmp.xml", filePath))
    {
        Global::saveErr.append(Global::prj_home_path + "/tmp/tmp.xml copy fail");
        return false;
    }
    return true;
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
    //                qDebug() << strlist2[0] << " camera_id: " << camera_id;
                    dtsFile->close();
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
 //               qDebug() << strLine;

                if(strLine.contains("compatible") || strLine.contains("//"))
                {
                    if(strLine.contains("//"))
                    {
                        strLine = ts.readLine();
                    }

                    QStringList strlist1 = strLine.split(",");
                    QString str2 = strlist1[1];
                    QStringList strlist2 = str2.split("-");
  //                  qDebug() << strlist2[0] << "camera_id: " << camera_id;
                    dtsFile->close();
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
    enableCam(curCamId, dtsPath);
}

bool TextHelper::disableCam(int camId, QString dtsPath)
{
    QFile   *dtsFile =  new QFile(dtsPath);
    QFile   *newDtsFile = new QFile(QDir::currentPath() + "/tmp/Sofia3GR-tablet.dts");
    QTextStream newDts(newDtsFile);
    QTextStream dtsOld(dtsFile);
    QString strLine;
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "disableCam : dtsFile open fail";
        return false;
    }
    if(!newDtsFile->open(QIODevice::WriteOnly))
    {
        qDebug() << "disableCam: newDtsFile open fail";
        dtsFile->close();
        return false;
    }
    while(!dtsOld.atEnd())
    {
        strLine = dtsOld.readLine();
        if(strLine.contains("camera" + QString::number(camId, 10)) && strLine.contains(":"))
        {
               newDts << strLine << "\n";
               newDts << "\t\tstatus = \"disabled\";\n";
               strLine = dtsOld.readLine();
               continue;
        }
        newDts << strLine << "\n";
    }
    dtsFile->close();
    newDtsFile->flush();
    newDtsFile->close();

    if(!dtsFile->remove())
    {
        qDebug() << "remove dtsFile fail disableCam";
        return false;
    }
    if(!newDtsFile->copy(dtsPath))
    {
        qDebug() << "camId: " << camId << "file copy fail!!! (disableCam)";
    }

    return true;
}

bool TextHelper::enableCam(int camId, QString dtsPath)
{
    QFile   *dtsFile =  new QFile(dtsPath);
    QFile   *newDtsFile = new QFile(QDir::currentPath() + "/tmp/Sofia3GR-tablet.dts");
    QTextStream newDts(newDtsFile);
    QTextStream dtsOld(dtsFile);
    QString strLine;
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "dtsFile open fail  enableCam";
        return false;
    }
    if(!newDtsFile->open(QIODevice::WriteOnly))
    {
        qDebug() << "newDtsFile open fail  enableCam";
        dtsFile->close();
        return false;
    }
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

    if(!dtsFile->remove())
    {
        qDebug() << "remove dtsFile fail enableCam";
        return false;
    }
    if(!newDtsFile->copy(dtsPath))
    {
        qDebug() << "camId: " << camId << "file copy fail!!!(enableCam)";
        return false;
    }
    return true;
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
        Global::saveErr.append(filePath + "open fail\n");
        return false;
    }
    if(!tempFile->open(QIODevice::WriteOnly))
    {
        qDebug() << filePath << ": tempFile open fail";
        Global::saveErr.append(QDir::currentPath() + "/tmp/temp.txt open fail\n");
        oriFile->close();
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
            if(split == "&emmc ")
            {
                tempTS << strLine << "\n";
                for(int i = 0; i < 5; i++)
                {
                    strLine = oriTS.readLine();
                    tempTS << strLine << "\n";
                }
                strLine = oriTS.readLine();
                if(value == "disabled")
                {
                    tempTS << "  status = \"disabled\";\n";
                    for(int i = 0; i < 7; i++)
                    {
                        strLine = oriTS.readLine();
                        tempTS << strLine << "\n";
                    }
                    tempTS << "  status = \"okay\";\n";
                    strLine = oriTS.readLine();
                    continue;
                }else
                {
                    tempTS << "  status = \"okay\";\n";
                    for(int i = 0; i < 7; i++)
                    {
                        strLine = oriTS.readLine();
                        tempTS << strLine << "\n";
                    }
                    tempTS << "  status = \"disabled\";\n";
                    strLine = oriTS.readLine();
                    continue;
                }
            }
            tempTS << str << split << value << "\n";
//            qDebug() << "tempTS: " << str << split << value;
            continue;
        }
        tempTS << strLine << "\n";
    }
    oriFile->close();
    tempFile->flush();
    tempFile->close();
    if(!oriFile->remove())
    {
        qDebug() << "oriFile remove fail";
        Global::saveErr.append(filePath + "remove fail \n");
        return false;
    }
    if(!tempFile->copy(filePath))
    {
        qDebug() << "copy fail" << filePath << "  str: " << str;
        Global::saveErr.append(QDir::currentPath() + "/tmp/temp.txt  override fail\n");
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
        wallpaperXml.close();
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
   if(!wallpaperXml.remove())
   {
        qDebug() << "remove wallpaperXml fail";
        return false;
   }
    if(!tempXml.copy(filePath))
    {
        qDebug() << "addWallpaperXml: str: " << newStr << "file copy fail!!!";
        return false;
    }
    return true;
}
/*
 * 默认前置为gc0310，但是dts文件0310选项没有status一行，需要加上status一行，和其他摄像头选项保持一致
 *
 */
bool TextHelper::addState2Gc0310Dts(QString dtsPath)
{
    QFile   *dtsFile =  new QFile(dtsPath);
    QFile   *newDtsFile = new QFile(QDir::currentPath() + "/tmp/Sofia3GR-tablet.dts");
    QTextStream newDts(newDtsFile);
    QTextStream dtsOld(dtsFile);
    QString strLine;
    int frontCamId = readCam("front", dtsPath) + 6;
//  begin------      if gc0310 already add status switch, return true
    {
        if(!dtsFile->open(QIODevice::ReadOnly))
        {
            qDebug() << dtsPath << ": open fail --gc0310 already add status switch";
            return false;
        }
        while(!dtsOld.atEnd())
        {
            strLine = dtsOld.readLine();
            if(strLine.contains("camera-module@7"))
            {  
                strLine = dtsOld.readLine();
                if(strLine.contains("status"))
                {
                    dtsFile->close();
                    qDebug() << "gc0310 already add status";
                    return true;
                }else
                {
                    qDebug() << "gc0310 not add status";
                    dtsFile->close();
                    break;
                }
            }
        }
    dtsFile->close();
    }
 //  end  --------------------------------------------------------
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        qDebug() << " dtsFile open fail.....";
        return false;
    }
    if(!newDtsFile->open(QIODevice::WriteOnly))
    {
        qDebug() << " newDtsFile open fail";
        dtsFile->close();
        return false;
    }
    dtsOld.seek(0);
    newDts.seek(0);
    while(!dtsOld.atEnd())
    {
        strLine = dtsOld.readLine();
        if(strLine.contains("camera-module@7"))
        {
            newDts << strLine << "\n";
            strLine = dtsOld.readLine();
            if(strLine.contains("status"))
            {
                dtsFile->close();
                newDtsFile->close();
                return true;
            }else
            {
                if(frontCamId == 7)
                {
                    newDts << "\t\t//status = \"disabled\";\n";
                    newDts << strLine << "\n";
                    continue;
                }else
                {
                    newDts << "\t\tstatus = \"disabled\";\n";
                    newDts << strLine << "\n";
                    continue;
                }
            }
         }
        newDts << strLine << "\n";
    }
    dtsFile->close();
    newDtsFile->flush();
    newDtsFile->close();
    if(dtsFile->remove())
    {
        if(!newDtsFile->copy(dtsPath))
        {
            qDebug() << "dtsFile copy fail";
            return false;
        }
    }else
    {
        qDebug() << "dtsFile remove fail :: " << dtsPath;
        dtsFile->close();
        return false;
    }
    return true;
}

QStringList TextHelper::readBatteryPar()
{
    QFile *dtsFile = new QFile(Global::srcPath + "/" + Global::dtsPath);
    if(!dtsFile->open(QIODevice::ReadOnly))
    {
        return NULL;
    }
    QString strLine;
    while(!dtsFile->atEnd())
    {
        strLine = dtsFile->readLine();
        if(strLine.contains("battery {"))
        {
asdfsf
        }
    }
}



