// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <iomanip>
#include <vw/Core/ProgressCallback.h>

namespace {
  vw::ProgressCallback g_dummy_progress_callback_instance;
}

const vw::ProgressCallback &vw::ProgressCallback::dummy_instance() {
  return g_dummy_progress_callback_instance;
}

void vw::TerminalProgressCallback::print_progress() const {
  if (fabs(m_progress - m_last_reported_progress) > m_step) {
    m_last_reported_progress = m_progress;
    int pi = static_cast<int>(m_progress * 60);
    std::ostringstream p;
    p << "\r" << m_pre_progress_text << "[";
    for( int i=0; i<pi; ++i ) p << "*";
    for( int i=60; i>pi; --i ) p << ".";
    p << "] " << std::setprecision(m_precision) << std::fixed << (m_progress*100.0) << "%";
    std::cerr << p.str() << std::flush;
  }
}

void vw::TerminalProgressCallback::report_aborted(std::string why) const {
  Mutex::Lock lock(m_mutex);
  vw_out(m_level) << " Aborted: " << why << std::endl;
}

void vw::TerminalProgressCallback::report_finished() const {
  Mutex::Lock lock(m_mutex);
  vw_out(m_level) << "\r" << m_pre_progress_text << "[************************************************************] Complete!\n";
}
