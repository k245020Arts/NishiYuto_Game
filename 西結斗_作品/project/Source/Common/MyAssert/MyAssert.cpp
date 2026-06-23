#include "MyAssert.h"

void ErrorAssert(std::string_view errMsg, const char* file, int line)
{
	//Release‚É‚Íwassert‚Í‚È‚¢
#ifdef _DEBUG

    // errMsg ‚Í string_view ‚È‚Ì‚ÅˆÀ‘S‚É std::string ‚É•ÏŠ·
    std::wstring wErrStr = sjis_to_wide(std::string(errMsg));
    std::wstring wErrFile = sjis_to_wide(file);

    // ˆÀ‘S‚És”Ô†‚ğ“n‚·
    _wassert(wErrStr.c_str(), wErrFile.c_str(), static_cast<unsigned int>(line));
#endif // DEBUG
}
