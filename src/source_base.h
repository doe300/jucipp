#pragma once

#include <boost/filesystem.hpp>
#include <gtksourceviewmm.h>
#include <mutex>
#include <set>

namespace Source {
  class BaseView : public Gsv::View {
  public:
    BaseView(const boost::filesystem::path &file_path, const Glib::RefPtr<Gsv::Language> &language);
    ~BaseView() override;
    boost::filesystem::path file_path;

    Glib::RefPtr<Gsv::Language> language;

    bool load(bool not_undoable_action = false);
    /// Set new text more optimally and without unnecessary scrolling
    void replace_text(const std::string &new_text);
    virtual void rename(const boost::filesystem::path &path);
    virtual bool save() = 0;

    Glib::RefPtr<Gio::FileMonitor> monitor;
    sigc::connection monitor_changed_connection;
    sigc::connection delayed_monitor_changed_connection;

    virtual void configure() = 0;
    virtual void hide_tooltips() = 0;
    virtual void hide_dialogs() = 0;

    void set_tab_char_and_size(char tab_char, unsigned tab_size);
    std::pair<char, unsigned> get_tab_char_and_size() { return {tab_char, tab_size}; }

    /// Safely returns iter given line and an offset using either byte index or character offset. Defaults to using byte index.
    virtual Gtk::TextIter get_iter_at_line_pos(int line, int pos);
    /// Safely returns iter given line and character offset
    Gtk::TextIter get_iter_at_line_offset(int line, int offset);
    /// Safely returns iter given line and byte index
    Gtk::TextIter get_iter_at_line_index(int line, int index);

    Gtk::TextIter get_iter_at_line_end(int line_nr);
    Gtk::TextIter get_iter_for_dialog();

    /// Safely places cursor at line using get_iter_at_line_pos.
    void place_cursor_at_line_pos(int line, int pos);
    /// Safely places cursor at line offset
    void place_cursor_at_line_offset(int line, int offset);
    /// Safely places cursor at line index
    void place_cursor_at_line_index(int line, int index);

    /// Use with care, view could be destroyed while this functions is running!
    std::function<void(BaseView *view, bool center, bool show_tooltips)> scroll_to_cursor_delayed = [](BaseView *view, bool center, bool show_tooltips) {};

    std::function<void(BaseView *view)> update_tab_label;
    std::function<void(BaseView *view)> update_status_location;
    std::function<void(BaseView *view)> update_status_file_path;
    std::function<void(BaseView *view)> update_status_diagnostics;
    std::function<void(BaseView *view)> update_status_state;
    std::tuple<size_t, size_t, size_t> status_diagnostics;
    std::string status_state;
    std::function<void(BaseView *view)> update_status_branch;
    std::string status_branch;
    std::function<void(int number)> update_search_occurrences;

    void paste();

    void search_highlight(const std::string &text, bool case_sensitive, bool regex);
    void search_forward();
    void search_backward();
    void replace_forward(const std::string &replacement);
    void replace_backward(const std::string &replacement);
    void replace_all(const std::string &replacement);

    bool disable_spellcheck = false;

  private:
    GtkSourceSearchContext *search_context;
    GtkSourceSearchSettings *search_settings;
    static void search_occurrences_updated(GtkWidget *widget, GParamSpec *property, gpointer data);

  protected:
    std::time_t last_write_time;
    void monitor_file();
    void check_last_write_time(std::time_t last_write_time_ = static_cast<std::time_t>(-1));

    bool is_bracket_language = false;

    unsigned tab_size;
    char tab_char;
    std::string tab;
    std::pair<char, unsigned> find_tab_char_and_size();

    /// Apple key for MacOS, and control key otherwise
    GdkModifierType primary_modifier_mask;

    /// Move iter to line start. Depending on iter position, before or after indentation.
    /// Works with wrapped lines.
    Gtk::TextIter get_smart_home_iter(const Gtk::TextIter &iter);
    /// Move iter to line end. Depending on iter position, before or after indentation.
    /// Works with wrapped lines.
    /// Note that smart end goes FIRST to end of line to avoid hiding empty chars after expressions.
    Gtk::TextIter get_smart_end_iter(const Gtk::TextIter &iter);

    std::string get_line(const Gtk::TextIter &iter);
    std::string get_line(const Glib::RefPtr<Gtk::TextBuffer::Mark> &mark);
    std::string get_line(int line_nr);
    std::string get_line();
    std::string get_line_before(const Gtk::TextIter &iter);
    std::string get_line_before(const Glib::RefPtr<Gtk::TextBuffer::Mark> &mark);
    std::string get_line_before();
    Gtk::TextIter get_tabs_end_iter(const Gtk::TextIter &iter);
    Gtk::TextIter get_tabs_end_iter(const Glib::RefPtr<Gtk::TextBuffer::Mark> &mark);
    Gtk::TextIter get_tabs_end_iter(int line_nr);
    Gtk::TextIter get_tabs_end_iter();

    std::pair<Gtk::TextIter, Gtk::TextIter> get_token_iters(Gtk::TextIter iter);
    std::string get_token(const Gtk::TextIter &iter);
    void cleanup_whitespace_characters();
    void cleanup_whitespace_characters(const Gtk::TextIter &iter);

    bool enable_multiple_cursors = false;
  };
} // namespace Source
