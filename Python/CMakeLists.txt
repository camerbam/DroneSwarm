set(exe_name Python)
###################################
set(${exe_name}_srcs
  UDP.py
  TCP.py
  )
list(APPEND ${exe_name}_libsrcs ${${exe_name}_srcs})
source_group("" FILES ${${exe_name}_srcs})
####################################
xpSourceListAppend("${${exe_name}_libsrcs}")
