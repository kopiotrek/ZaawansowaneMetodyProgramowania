#include "klient.hh"

/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 1
 * i obrazujących jego ruch.
 */
const char *Cmds4Obj1[STATES_NUMBER] = {
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,23)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,26)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,29)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,32)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,35)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,38)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,41)\n",
    "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,44)\n"};

/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 2
 * i obrazujących jego ruch.
 */
const char *Cmds4Obj2[STATES_NUMBER] = {
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-48,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-45,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-42,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-39,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-36,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-33,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-30,0)\n",
    "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-27,0)\n"};

/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 3
 * i obrazujących jego ruch.
 */
const char *Cmds4Obj3[STATES_NUMBER] = {
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-48,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-51,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-54,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-57,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-60,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-63,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-66,0)\n",
    "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-69,0)\n"};

/*!
 * \brief Wysyła napis do poprzez gniazdo sieciowe.
 *
 * Wysyła napis do poprzez gniazdo sieciowe.
 * \param[in] Sk2Server - deskryptor gniazda sieciowego, poprzez które
 *                        ma zostać wysłany napis w sensie języka C.
 * \param[in] sMesg - zawiera napis, który ma zostać wysłany poprzez
 *                    gniazdo sieciowe.
 */
int Send(int Sk2Server, const char *sMesg)
{
  ssize_t IlWyslanych;
  ssize_t IlDoWyslania = (ssize_t)strlen(sMesg);

  while ((IlWyslanych = write(Sk2Server, sMesg, IlDoWyslania)) > 0)
  {
    IlDoWyslania -= IlWyslanych;
    sMesg += IlWyslanych;
  }
  if (IlWyslanych < 0)
  {
    cerr << "*** Blad przeslania napisu." << endl;
  }
  return 0;
}

/*!
 * \brief Funkcja jest treścią wątku komunikacyjnego
 *
 * Funkcja jest treścią wątku komunikacyjnego.
 * \param[in] rSender - odpowiada za śledenie zmian na scenie
 *                      i przesyłanie do serwera graficznego
 *                      aktualnego stanu sceny, gdy uległ on zmianie.
 */
void Fun_CommunicationThread(Sender *pSender)
{
  pSender->Watching_and_Sending();
}

/*!
 * Otwiera połączenie sieciowe
 * \param[out]  rSocket - deskryptor gniazda, poprzez które może być
 *                        realizowana komunikacja sieciowa.
 */
bool OpenConnection(int &rSocket)
{
  struct sockaddr_in DaneAdSerw;

  bzero((char *)&DaneAdSerw, sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);

  rSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (rSocket < 0)
  {
    cerr << "*** Blad otwarcia gniazda." << endl;
    return false;
  }

  if (connect(rSocket, (struct sockaddr *)&DaneAdSerw, sizeof(DaneAdSerw)) < 0)
  {
    cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << endl;
    return false;
  }
  return true;
}

/*!
 * \brief Przykład wykonywania pojedynczej operacji z animacją.
 *
 * Przykład wykonywania pojedynczej operacji z animacją.
 * \param[in] pObj - wskaźnik na obiekt, dla którego ma być wykonana dana operacja
 *                   zmiany jego stanu.
 * \param[in] pAccCtrl - wskaźnik na obiekt, poprzez który przekazywana jest informacja
 *                   o zmianie na scenie, zaś w trakcie dokonywania zmianay
 *                   zamykany jest dostęp do całej sceny.
 * \retval true - Jeśli dokonan zosała zmiana stanu wszystkich obiektów.
 * \retval false - w przypadku przeciwnym.
 */
bool ChangeState(Scene &Scn) // GeomObject *pObj, AccessControl  *pAccCtrl)
{
  bool Changed;

  while (true)
  {
    Scn.LockAccess(); // Zamykamy dostęp do sceny, gdy wykonujemy
                      // modyfikacje na obiekcie.
    for (GeomObject &rObj : Scn._Container4Objects)
    {
      if (!(Changed = rObj.IncStateIndex()))
      {
        Scn.UnlockAccess();
        return false;
      }
    }
    Scn.MarkChange();
    Scn.UnlockAccess();
    usleep(300000);
  }
  return true;
}

int klient(Configuration &rConfig)
{
  cout << "Port: " << PORT << endl;
  Scene Scn;
  int Socket4Sending;

  if (!OpenConnection(Socket4Sending))
    return 1;

  Sender ClientSender(Socket4Sending, &Scn);
  //  thread   Thread4Sending(Fun_Sender, Socket4Sending, &ClientSender);

  thread Thread4Sending(Fun_CommunicationThread, &ClientSender);
  const char *sConfigCmds =
      "Clear\n"
      "AddObj Name=Podstawa1 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,20) Trans_m=(-1,3,0)\n"
      "AddObj Name=Podstawa1.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
      "AddObj Name=Podstawa1.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n"
      "AddObj Name=Podstawa2 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(-1,-3,0)\n"
      "AddObj Name=Podstawa2.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
      "AddObj Name=Podstawa2.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n";

  std::vector<Cube> Cubes = rConfig.getCubes();
  const char *AttrNames[] = {" Shift=", " Scale=", " RotXYZ_deg=", " Trans_m=", " RGB="};
  for (int i = 0; i < size(Cubes); i++)
  {
    cout << "Kostka nr: " << i << " Name: " << Cubes[i].getName() << endl;
    std::vector<Vector3D> attr = Cubes[i].getAttributes();
    for (int j = 0; j < 5; j++)
    {
      cout << AttrNames[j] << attr[j];
    }
    cout << endl;
  }

  cout << "Konfiguracja:" << endl;
  cout << sConfigCmds << endl;

  Send(Socket4Sending, sConfigCmds);

  cout << "Akcja:" << endl;
  for (GeomObject &rObj : Scn._Container4Objects)
  {
    usleep(20000);
    ChangeState(Scn);
    Scn.MarkChange();
    usleep(100000);
  }
  usleep(100000);

  //-------------------------------------
  // Należy pamiętać o zamknięciu połączenia.
  // Bez tego serwer nie będzie reagował na
  // nowe połączenia.
  //
  cout << "Close\n"
       << endl; // To tylko, aby pokazac wysylana instrukcje
  Send(Socket4Sending, "Close\n");
  ClientSender.CancelCountinueLooping();
  Thread4Sending.join();
  close(Socket4Sending);
}
