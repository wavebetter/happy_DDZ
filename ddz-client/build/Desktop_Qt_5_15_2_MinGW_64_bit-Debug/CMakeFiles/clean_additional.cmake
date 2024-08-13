# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ddz-client_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ddz-client_autogen.dir\\ParseCache.txt"
  "card\\CMakeFiles\\card_autogen.dir\\AutogenUsed.txt"
  "card\\CMakeFiles\\card_autogen.dir\\ParseCache.txt"
  "card\\card_autogen"
  "control\\CMakeFiles\\control_autogen.dir\\AutogenUsed.txt"
  "control\\CMakeFiles\\control_autogen.dir\\ParseCache.txt"
  "control\\control_autogen"
  "crypto\\CMakeFiles\\crypto_autogen.dir\\AutogenUsed.txt"
  "crypto\\CMakeFiles\\crypto_autogen.dir\\ParseCache.txt"
  "crypto\\crypto_autogen"
  "ddz-client_autogen"
  "player\\CMakeFiles\\player_autogen.dir\\AutogenUsed.txt"
  "player\\CMakeFiles\\player_autogen.dir\\ParseCache.txt"
  "player\\player_autogen"
  "serialize\\CMakeFiles\\serialize_autogen.dir\\AutogenUsed.txt"
  "serialize\\CMakeFiles\\serialize_autogen.dir\\ParseCache.txt"
  "serialize\\serialize_autogen"
  "strategy\\CMakeFiles\\strategy_autogen.dir\\AutogenUsed.txt"
  "strategy\\CMakeFiles\\strategy_autogen.dir\\ParseCache.txt"
  "strategy\\strategy_autogen"
  "tcp\\CMakeFiles\\tcp_autogen.dir\\AutogenUsed.txt"
  "tcp\\CMakeFiles\\tcp_autogen.dir\\ParseCache.txt"
  "tcp\\tcp_autogen"
  "thread\\CMakeFiles\\thread_autogen.dir\\AutogenUsed.txt"
  "thread\\CMakeFiles\\thread_autogen.dir\\ParseCache.txt"
  "thread\\thread_autogen"
  "windows\\CMakeFiles\\windows_autogen.dir\\AutogenUsed.txt"
  "windows\\CMakeFiles\\windows_autogen.dir\\ParseCache.txt"
  "windows\\windows_autogen"
  )
endif()
