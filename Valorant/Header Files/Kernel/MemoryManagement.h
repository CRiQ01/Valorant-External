#pragma once
#include "KernelMode.h"
#include <TlHelp32.h>
#include <vector>
class MemoryManagement
{
public:
	HANDLE hDevice;
	uint64_t ModuleBase;
	bool Init(string ProcessName);
	BOOL Read_Buf(ULONGLONG lpBaseAddress, PVOID buf, ULONG bufersize);
	uint32_t GetPIDByProcessName(string ProcessName);
	uint64_t GetModuleBase();
	void SetGuardedRegion();
	BOOL ReadGuardedRegion(
		ULONG_PTR Displacement,
		PVOID pBuffer,
		ULONG cbBuffer);
	BOOL ReadVirtualMemory(
		ULONG_PTR Address,
		PVOID pBuffer,
		ULONG cbBuffer);
};
inline bool MemoryManagement::Init(string ProcessName)
{
	DWORD Bytes;
	this->hDevice = CreateFileW(DRIVER_DEVICE_PATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (this->hDevice == INVALID_HANDLE_VALUE)
		return 0;;
	_DRIVER_INIT tmp;
	tmp.ProcessId = this->GetPIDByProcessName(ProcessName);
	if (!tmp.ProcessId)
		return false;
	DeviceIoControl(this->hDevice, IOCTL_DRIVER_INIT, &tmp, sizeof(tmp), &tmp, sizeof(tmp), &Bytes, 0);
	this->ModuleBase = this->GetModuleBase();
	if (this->ModuleBase)
		return true;
	else
		return false;
}
inline BOOL MemoryManagement::ReadVirtualMemory(
	ULONG_PTR Address,
	PVOID pBuffer,
	ULONG cbBuffer)
{
	if (Address) {
		READ_VIRTUAL_MEMORY_REQUEST Request = {};
		DWORD cbReturned = 0;
		BOOL status = TRUE;

		RtlSecureZeroMemory(pBuffer, cbBuffer);

		Request.Address = Address;
		Request.Size = cbBuffer;

		status = DeviceIoControl(
			this->hDevice,
			IOCTL_DRIVER_MANAGE_MEMORY,
			&Request,
			sizeof(Request),
			pBuffer,
			cbBuffer,
			&cbReturned,
			NULL);
		if (!status)
		{
			goto exit;
		}

	exit:
		return status;
	}
}

inline BOOL MemoryManagement::Read_Buf(DWORD_PTR lpBaseAddress, PVOID buf, ULONG bufersize)
{
	DWORD Bytes;
	KERNEL_READ_REQUEST copy = {};
	RtlSecureZeroMemory(buf, bufersize);
	copy.TargetAddress = lpBaseAddress;
	copy.ResponseAddress = (DWORD_PTR)buf;
	copy.Size = bufersize;
	if (DeviceIoControl(this->hDevice, IOCTL_DRIVER_MANAGE_MEMORY, &copy, sizeof(copy), &copy, sizeof(copy), &Bytes, NULL))
		return TRUE;
	else
		return FALSE;
}

inline uint32_t MemoryManagement::GetPIDByProcessName(string ProcessName)
{
	DWORD Result = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);
	do
		if (!strcmp(ProcEntry.szExeFile, ProcessName.c_str()))
		{
			Result = ProcEntry.th32ProcessID;
		}
	while (Process32Next(hSnap, &ProcEntry));
	if (hSnap)
		CloseHandle(hSnap);
	return Result;
}
inline uint64_t MemoryManagement::GetModuleBase()
{
	DWORD Bytes;
	_GET_BASE_ADDRESS ModuleBase;
	uint64_t ModulePtr = 0;
	ModuleBase.Result = &ModulePtr;
	DeviceIoControl(this->hDevice, IOCTL_DRIVER_GET_BASE_ADDRESS, &ModuleBase, sizeof(ModuleBase), &ModuleBase, sizeof(ModuleBase), &Bytes, 0);
	return ModulePtr;
}

inline void MemoryManagement::SetGuardedRegion()
{
	DeviceIoControl(this->hDevice, IOCTL_SET_GUARDED_REGION, 0, 0, 0, 0, 0, 0);
}

inline BOOL MemoryManagement::ReadGuardedRegion(
	ULONG_PTR Displacement,
	PVOID pBuffer,
	ULONG cbBuffer)
{
	READ_GUARDED_REGION_REQUEST Request = {};
	DWORD cbReturned = 0;
	BOOL status = TRUE;

	if (Displacement > 0x200000)
	{
		status = FALSE;
		goto exit;
	}

	RtlSecureZeroMemory(pBuffer, cbBuffer);

	Request.Displacement = Displacement;
	Request.Buffer = pBuffer;
	Request.Size = cbBuffer;
	status = DeviceIoControl(
		this->hDevice,
		IOCTL_READ_GUARDED_REGION,
		&Request,
		sizeof(Request),
		&Request,
		sizeof(Request),
		&cbReturned,
		NULL);
	if (!status)
	{
		goto exit;
	}

exit:
	return status;
}