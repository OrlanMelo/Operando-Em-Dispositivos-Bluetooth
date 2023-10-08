#include <Windows.h>
#include <bluetoothapis.h>
#include <string>
#include <iostream>
#include <fstream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"bthprops.lib")

using namespace std;

class cFuncoes
{
private:

	BLUETOOTH_DEVICE_INFO Dispositivo;
	BLUETOOTH_DEVICE_SEARCH_PARAMS Pesquisa;

	HBLUETOOTH_DEVICE_FIND Find1;
	BLUETOOTH_FIND_RADIO_PARAMS pRadio;
	HANDLE hRadio;

public:

	void EnumerarRadiosBluetooth()
	{
		int Quantidade = 0;

		HBLUETOOTH_RADIO_FIND Radio;
		Radio = BluetoothFindFirstRadio(&pRadio, &hRadio);
		if (Radio == (HANDLE)ERROR_NO_MORE_ITEMS)
		{
			Quantidade -= 1;
			cout << "Não foi encontrado nenhum rádio bluetooth..\n";
		}
		else if (Radio == (HANDLE)ERROR_OUTOFMEMORY)
		{
			Quantidade -= 1;
			cout << "Não há memória suficiente para o uso da função..\n";
		}

		do
		{
			Quantidade += 1;
		} while (BluetoothFindNextRadio(Radio,&hRadio));

		cout << "Rádios bluetooth encontrados: " << Quantidade;

		BluetoothFindRadioClose(Radio);
	}

	void EnumerarDispositivosBluetooth()
	{
		int Quantidade = 0;

		BluetoothEnableDiscovery(0, TRUE);//Habilite a descoberta para que este dispositivo seja localizado.

		Pesquisa.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
		Dispositivo.dwSize = sizeof(Dispositivo);

		Pesquisa.fReturnAuthenticated = FALSE;//Autenticado.
		Pesquisa.fReturnConnected = TRUE;//Conectado.
		Pesquisa.fReturnRemembered = TRUE;//Relembrado.
		Pesquisa.fReturnUnknown = TRUE;//Desconhecido.
		Pesquisa.hRadio = NULL;
		Pesquisa.cTimeoutMultiplier = 20;//Tempo, o tamanho máximo é 48.
		Pesquisa.fIssueInquiry = TRUE;//Nova busca após a primeira tentativa, usando o mesmo tempo fornecido.

		Find1 = BluetoothFindFirstDevice(&Pesquisa, &Dispositivo);
		if (Find1 == NULL)
		{
			cout << "Não foram encontrados dispositivos bluetooth..\n";
			Quantidade -= 1;
		}
		do
		{
			Quantidade += 1;

			_tprintf(TEXT("Dispositivo: %s\n"), Dispositivo.szName);
			cout << "Endereço: " << Dispositivo.Address.ullLong << '\n';
			
			if (Dispositivo.fConnected == TRUE)
				cout << "Status: Conectado ao dispositivo\n";
			else
				cout << "Status: Não conectado\n";

			cout << "\n";

		} while (BluetoothFindNextDevice(Find1, &Dispositivo));

		cout << "A busca terminou..\n";

		cout << "Encontrados: " << Quantidade << " dispositivos..\n";

		BluetoothFindDeviceClose(Find1);
	}

	void ConectarDispositivoBluetoothPorPerto()
	{

		BluetoothGetDeviceInfo(Find1, &Dispositivo);

		DWORD Res;
		PWSTR Senha = L"1234";//O sistema irá determinar automaticamente.
		Res = BluetoothAuthenticateDevice(0, 0, &Dispositivo, Senha, 4);
	}

}Funcoes;

int main()
{

	cout << "O assistente está executando pesquisas e operações para dispositivos bluetooth...";

	Funcoes.EnumerarRadiosBluetooth();
	Funcoes.EnumerarDispositivosBluetooth();
	Funcoes.ConectarDispositivoBluetoothPorPerto();//O mais próximo será notificado.

	system("pause");
}