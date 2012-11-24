#pragma once

enum  
{
    M68K_LOG_DEBUG, 
    M68K_LOG_INFO, 
    M68K_LOG_ERROR, 
};

void m68k_log(int logLevel, const char* format, ...);
void m68k_log_set_level(int logLevel);
void m68k_log_level_push();
void m68k_log_level_pop();

