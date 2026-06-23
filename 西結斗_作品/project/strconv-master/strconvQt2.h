/* strconvQt2.h v1.0.0             */
/* Last Modified: 2024/10/10 01:00 */
#ifndef STRCONVQT2_H
#define STRCONVQT2_H

#include <QString>
#include "strconv2.h"

static inline QString wide_to_qstr(const std::wstring &s)
{
  return QString::fromStdWString(s);
}
static inline std::wstring qstr_to_wide(const QString &s)
{
  return s.toStdWString();
}

static inline QString ansi_to_qstr(const std::string &s)
{
  return QString::fromStdWString(cp_to_wide(s, CP_ACP));
}
static inline std::string qstr_to_ansi(const QString &s)
{
  return wide_to_cp(s.toStdWString(), CP_ACP);
}

static inline QString sjis_to_qstr(const std::string &s)
{
  return QString::fromStdWString(cp_to_wide(s, 932));
}
static inline std::string qstr_to_sjis(const QString &s)
{
  return wide_to_cp(s.toStdWString(), 932);
}

static inline QString utf8_to_qstr(const std::string &s)
{
  return QString::fromStdWString(cp_to_wide(s, CP_UTF8));
}
static inline std::string qstr_to_utf8(const QString &s)
{
  return wide_to_cp(s.toStdWString(), CP_UTF8);
}

#endif /* STRCONVQT2_H */
