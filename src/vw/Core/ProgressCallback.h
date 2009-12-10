// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


/// \file ProgressCallback.h
/// 
/// A class for monitoring the progress of lengthy operations.
///
#ifndef __VW_CORE_PROGRESSCALLBACK_H__
#define __VW_CORE_PROGRESSCALLBACK_H__

#include <math.h>
#include <string>

#include <vw/Core/CoreExport.h>
#include <vw/Core/Log.h>
#include <vw/Core/Exception.h>
#include <vw/Core/Thread.h>

#include <vw/config.h>

namespace vw {

  /// The base class for progress monitoring.
  class VW_CORE_DECL ProgressCallback {
  protected:
    // WARNING:  These may not be valid for some subclasses.  Always access these 
    // values through the relevant public functions unless you know what you're doing.
    // Arguably having every single member variable be mutable suggests a design 
    // flaw.  The idea is that we often want to create a temporary progress callback 
    // object to pass to a function performing some complex task, but that requires 
    // that all the key functions be const.  This isn't pretty, but it works for now.
    mutable bool m_abort_requested;
    mutable double m_progress;
    mutable Mutex m_mutex;

  public:
    ProgressCallback() : m_abort_requested( false ), m_progress(0) {}
    ProgressCallback( const ProgressCallback& copy ) {
      m_progress = copy.progress();
      m_abort_requested = copy.abort_requested();
    }

    // Reporting functions
    // Subclasses should reimplement where appropriate
    //
    // progress is from 0 (not done) to 1 (finished)
    // 
    virtual void report_progress(double progress) const { 
      Mutex::Lock lock(m_mutex);
      m_progress = progress; 
    }

    virtual void report_incremental_progress(double incremental_progress) const { 
      Mutex::Lock lock(m_mutex);
      m_progress += incremental_progress; 
    }

    virtual void report_aborted(std::string /*why*/="") const {}
    virtual void report_finished() const { 
      Mutex::Lock lock(m_mutex);
      m_progress = 1.0; 
    }

    // Helper method which computes progress and calls report_progress
    void report_fractional_progress(double n, double total) const {
      report_progress(fabs(total) < 1e-30 ? 0 : n/total);
    }

    // Has an abort been requested?
    virtual bool abort_requested() const { 
      Mutex::Lock lock(m_mutex);
      return m_abort_requested; 
    }

    // Throw vw::Aborted if abort has been requested
    void abort_if_requested() const {
      if (abort_requested()) {
        report_aborted();
        vw_throw(Aborted());
      }
    }

    // Request abort
    virtual void request_abort() const { 
      Mutex::Lock lock(m_mutex);
      m_abort_requested = true; 
    }

    virtual double progress() const { return m_progress; }

    virtual ~ProgressCallback() {}
    static const ProgressCallback &dummy_instance();
  };


  /// Monitors the progress of a subtask of a task.
  class VW_CORE_DECL SubProgressCallback : public ProgressCallback {
  protected:
    const ProgressCallback &m_parent;
    const double m_from;
    const double m_to;
  public:
    SubProgressCallback(const ProgressCallback &parent,
                        double from, double to) :
      m_parent(parent), m_from(from), m_to(to) {}
    virtual void report_progress(double progress) const {
      double parent_progress = m_from + (m_to - m_from)*progress;
      m_parent.report_progress(parent_progress);
    }
    virtual void report_incremental_progress(double incremental_progress) const { 
      double parent_progress = (m_to - m_from)*incremental_progress;
      m_parent.report_incremental_progress(parent_progress);
    }
    virtual void report_aborted(std::string why="") const {
      m_parent.report_aborted(why);
    }
    virtual bool abort_requested() const { return m_parent.abort_requested(); }
    virtual void request_abort() const { m_parent.request_abort(); }
    virtual ~SubProgressCallback() {}
    virtual double progress() const { return (m_parent.progress() - m_from) / (m_to - m_from); }
    double from() const { return m_from; }
    double to() const {return m_to; }
    const ProgressCallback& parent() const { return m_parent; }
  };


  /// A progress monitor that prints a progress bar on STDOUT.
  class VW_CORE_DECL TerminalProgressCallback : public ProgressCallback {
    MessageLevel m_level;
    std::string m_pre_progress_text;
    mutable double m_last_reported_progress;
    uint32_t m_precision;
    double  m_step;

  public:
    TerminalProgressCallback( MessageLevel level = InfoMessage, std::string pre_progress_text = "", uint32_t precision = 0) :
      m_level(level), m_pre_progress_text(pre_progress_text), m_last_reported_progress(-1), m_precision(precision), m_step(::pow(10., -(int32_t(precision)+2)))  {}
    virtual ~TerminalProgressCallback() {}

    TerminalProgressCallback( const TerminalProgressCallback& copy ) : ProgressCallback(copy) {
      m_level = copy.message_level();
      m_pre_progress_text = copy.pre_progress_text();
      m_progress = copy.progress();
      m_abort_requested = copy.abort_requested();
    }

    void set_progress_text( std::string const& text ) {
      Mutex::Lock lock(m_mutex);
      m_pre_progress_text = text;
    }

    virtual void report_progress(double progress) const {
      Mutex::Lock lock(m_mutex);
      m_progress = progress;
      print_progress();
    }

    virtual void report_incremental_progress(double incremental_progress) const {
      Mutex::Lock lock(m_mutex);
      m_progress += incremental_progress;
      print_progress();
    }

    virtual void report_aborted(std::string why="") const;
    virtual void report_finished() const;

    void print_progress() const;

    std::string pre_progress_text() const { return m_pre_progress_text; }
    MessageLevel message_level() const { return m_level; }
  };

} // namespace vw

#endif // __VW_CORE_FUNDAMENTALTYPES_H__
