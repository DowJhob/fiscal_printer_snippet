/////////  Получить время начала смены в фискальнике
//// возвращает дату время открытия сессии
   QDateTime get_last_session_start_time()
   {
       t.start();
       kkm.FNGetStatus();
       if ( kkm.Get_FNSessionState() == 1 )
       {
           qDebug()  << "Session open: " ;
           qDebug()  << "GetECRStatus: " << kkm.GetECRStatus();            //это что бы получить номер посл документа
           uint last_doc = kkm.Get_OpenDocumentNumber();                    //номер последнего документа
           QDateTime d = QDateTime::currentDateTime();
           d = d.addDays( -1 );
           qDebug()  << "currentDateTime: " << d;
           uint i_doc = p_dv( last_doc, d );
           //uint i_doc = p_dv( last_doc, QDateTime::fromString("M11d28y201923:01:02", "'M'M'd'd'y'yyyyhh:mm:ss") );
           qDebug() << "doc num close to date: " << i_doc ;
           while( last_doc >= i_doc )
           {
               kkm.Set_DocumentNumber( last_doc );
               kkm.FNFindDocument();
               if ( kkm.Get_DocumentType() == 2 )        //тип документа отчет об открытии смены
               {
                   qDebug()  << "time: " << t.nsecsElapsed()/1000000 << "." << t.nsecsElapsed()%1000000 << "ms";
//                    qDebug()  << "Get_Date: " << kkm.Get_Date() << " | Get_Time: " << kkm.Get_Time();
                   return kkm.Get_Time();
               }
               else
                   last_doc--;
           }
       }
       else qDebug()  << "Session closed: " ;
       QDateTime f;
       f.setTime_t(1);
       return f;
   }
   //// возвращает максимально близкий слева (ранее указанной) к указанной дате номер документа
   uint p_dv( uint  last_doc_num, QDateTime time)
   {
       uint i=0, m;
       while( i < last_doc_num )
       {
           m=( i + last_doc_num ) / 2;
           //==============================================================================
           kkm.Set_DocumentNumber( m );
           kkm.FNFindDocument();
           QDateTime cur_doc_time = kkm.Get_Time();
           qDebug() << "cur m: " << m << " | current doc time: " << cur_doc_time;
           //==============================================================================
           if ( time > cur_doc_time )
               i = m + 1;
           else last_doc_num = m;
       }
       return i;
   }
