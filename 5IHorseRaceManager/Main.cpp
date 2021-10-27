
//
// Siv3D August 2016 v2 for Visual Studio 2019
// 
// Requirements
// - Visual Studio 2015 (v140) toolset
// - Windows 10 SDK (10.0.17763.0)
//

# include <Siv3D.hpp>


class RaceType {
public:
	enum class Type {
		Win,
		Place,
		Quinella,
		Exacta,
		None
	};

public:
	Type type;

public:
	RaceType() :
		type(Type::None)
	{}

public:
	RaceType(const Type& type) :
		type(type)
	{}

public:
	String to_string() {
		switch (type) {
		case Type::Win:
			return L"単勝";
		case Type::Place:
			return L"複勝";
		case Type::Quinella:
			return L"馬連";
		case Type::Exacta:
			return L"馬単";
		}
		return L"";
	}
};


class HorseName {
public:
	enum class Name {
		A,
		B,
		C,
		D,
		E,
		None
	};

public:
	Name name;

public:
	HorseName() :
		name(Name::None)
	{}

public:
	HorseName(const Name& name) :
		name(name)
	{}

public:
	String to_string() const {
		switch (name) {
		case Name::A:
			return L"A";
		case Name::B:
			return L"B";
		case Name::C:
			return L"C";
		case Name::D:
			return L"D";
		case Name::E:
			return L"E";
		}
		return L"";
	}
};


class BettingTicket {
public:
	RaceType race_type;
	Array<HorseName> horses_name;
	HorseName horse_name_exacta_winner;
	HorseName horse_name_exacta_runner_up;
	int32 betting_handle;

public:
	BettingTicket() :
		race_type(),
		horses_name(),
		horse_name_exacta_winner(),
		horse_name_exacta_runner_up(),
		betting_handle(0)
	{}

public:
	BettingTicket(const RaceType& race_type, const Array<HorseName>& horses_name, const HorseName& horse_name_exacta_winner, const HorseName& horse_name_exacta_runner_up, const int32& betting_handle) :
		race_type(race_type),
		horses_name(horses_name),
		horse_name_exacta_winner(horse_name_exacta_winner),
		horse_name_exacta_runner_up(horse_name_exacta_runner_up),
		betting_handle(betting_handle)
	{}
};


class OddsWin {
public:
	HorseName result;
	double odds;

public:
	OddsWin() :
		result(),
		odds(0.0)
	{}

public:
	OddsWin(const HorseName& horse_name, const double& odds) :
		result(horse_name),
		odds(odds)
	{}
};


class OddsPlace {
public:
	Array<HorseName> results;
	Array<double> odds;

public:
	OddsPlace() :
		results(3),
		odds(3)
	{}

public:
	OddsPlace(const Array<HorseName>& results, const Array<double>& odds) :
		results(results),
		odds(odds)
	{}
};


class OddsQuinella {
public:
	Array<HorseName> results;
	double odds;

public:
	OddsQuinella() :
		results(2),
		odds(0.0)
	{}

public:
	OddsQuinella(const Array<HorseName>& results, const double& odds) :
		results(results),
		odds(odds)
	{}
};



class OddsExacta {
public:
	Array<HorseName> results;
	double odds;

public:
	OddsExacta() :
		results(2),
		odds(0.0)
	{}

public:
	OddsExacta(const Array<HorseName>& results, const double& odds) :
		results(results),
		odds(odds)
	{}
};


class OddsLast {
public:
	OddsWin odds_win;
	OddsPlace odds_place;
	OddsQuinella odds_quinella;
	OddsExacta odds_exacta;

public:
	OddsLast() :
		odds_win(),
		odds_place(),
		odds_quinella(),
		odds_exacta()
	{}

public:
	OddsLast(const OddsWin& odds_win, const OddsPlace& odds_place, const OddsQuinella& odds_quinella, const OddsExacta& odds_exacta) :
		odds_win(odds_win),
		odds_place(odds_place),
		odds_quinella(odds_quinella),
		odds_exacta(odds_exacta)
	{}
};


class Clock {
private:
	String _text;
	Rect _rect;
	Array<Color> _rect_colors;
	const Font _font;
	const Point _font_pos_center;
	const Color _font_color;

public:
	Clock() :
		_text(L"--:--"),
		_rect(0, 0, Window::Size().x, Window::Size().y / 16),
		_rect_colors(4),
		_font(16, L"Rounded M+ 1m regular"),
		_font_pos_center(Window::Size().x / 8 * 7, Window::Size().y / 32),
		_font_color(Palette::Black)
	{
		init();
	}

public:
	void init() {
		_text = L"--:--";
		_rect = Rect(0, 0, Window::Size().x, Window::Size().y / 16);
		_rect_colors.clear();
		_rect_colors.resize(4);
		for (int32 index = 0; index < 4; index++) {
			_rect_colors.at(index) = Color(Palette::White, 255 / 3 * index);
		}
	}

public:
	void update() {
		if (DateTime::Now().milliseconds < 500) {
			_text = Format(L"{:02}:{:02}"_fmt, DateTime::Now().hour, DateTime::Now().minute);
		}
		else {
			_text = Format(L"{:02} {:02}"_fmt, DateTime::Now().hour, DateTime::Now().minute);
		}
	}

public:
	void draw() const {
		_rect.draw({ _rect_colors.at(0), _rect_colors.at(1), _rect_colors.at(2), _rect_colors.at(3) });
		_font.drawCenter(_text, _font_pos_center, _font_color);
	}
};


class SnackBar {
public:
	enum class Flag {
		Information,
		Error
	};

private:
	Array<String> _texts;
	Array<Flag> _flags;
	Rect _rect;
	Vec2 _rect_pos;
	const Vec2 _rect_pos_start;
	const Vec2 _rect_pos_end;
	Color _rect_color;
	const Color _rect_color_information;
	const Color _rect_color_error;
	const Font _font;
	Vec2 _font_pos_center;
	const Vec2 _font_pos_center_start;
	const Vec2 _font_pos_center_end;
	Stopwatch _stopwatch;
	const int32 _stopwatch_duration;

public:
	SnackBar() :
		_texts(),
		_flags(),
		_rect(0, Window::Size().y, Window::Size().x, Window::Size().y / 16),
		_rect_pos(0.0, Window::Size().y),
		_rect_pos_start(0.0, Window::Size().y),
		_rect_pos_end(0.0, Window::Size().y / 16.0 * 15.0),
		_rect_color(0, 155, 159),
		_rect_color_information(0, 155, 159),
		_rect_color_error(148, 35, 67),
		_font(16, L"Rounded M+ 1m regular"),
		_font_pos_center(Window::Center().x, Window::Size().y + (Window::Size().y / 16.0) / 2.0),
		_font_pos_center_start(Window::Center().x, Window::Size().y + (Window::Size().y / 16.0) / 2.0),
		_font_pos_center_end(Window::Center().x, Window::Size().y / 16.0 * 15.0 + (Window::Size().y / 16.0) / 2.0),
		_stopwatch(),
		_stopwatch_duration(6000)
	{
		init();
	}

public:
	void init() {
		_texts.clear();
		_flags.clear();
		_rect = Rect(0, Window::Size().y, Window::Size().x, Window::Size().y / 16);
		_rect_pos = Vec2(0.0, Window::Size().y);
		_rect_color = Color(0, 155, 159);
		_font_pos_center = Vec2(Window::Center().x, Window::Size().y + (Window::Size().y / 16.0) / 2.0);
		_stopwatch.reset();
	}

public:
	void update() {
		if (!_texts.empty() && !_flags.empty()) {
			if (!_stopwatch.isActive() || _stopwatch.isPaused()) {
				_stopwatch.start();
				switch (_flags.front()) {
				case Flag::Information:
					break;
				case Flag::Error:
					break;
				}
			}
			if (_stopwatch.ms() < 1000) {
				_rect_pos = EaseOut(_rect_pos_start, _rect_pos_end, Easing::Bounce, _stopwatch.ms() / 1000.0);
				_rect = Rect(_rect_pos.asPoint(), Window::Size().x, Window::Size().y / 16);
				_font_pos_center = EaseOut(_font_pos_center_start, _font_pos_center_end, Easing::Bounce, _stopwatch.ms() / 1000.0);
			}
			else if (1000 <= _stopwatch.ms() && _stopwatch.ms() < _stopwatch_duration - 1000) {
				_rect_pos = _rect_pos_end;
				_rect = Rect(_rect_pos.asPoint(), Window::Size().x, Window::Size().y / 16);
				_font_pos_center = _font_pos_center_end;
			}
			else if (_stopwatch_duration - 1000 <= _stopwatch.ms() && _stopwatch.ms() < _stopwatch_duration) {
				_rect_pos = EaseOut(_rect_pos_end, _rect_pos_start, Easing::Bounce, 1.0 - ((_stopwatch_duration - _stopwatch.ms()) / 1000.0));
				_rect = Rect(_rect_pos.asPoint(), Window::Size().x, Window::Size().y / 16);
				_font_pos_center = EaseOut(_font_pos_center_end, _font_pos_center_start, Easing::Bounce, 1.0 - ((_stopwatch_duration - _stopwatch.ms()) / 1000.0));
			}
			else {
				_texts.erase(_texts.begin());
				_flags.erase(_flags.begin());
				_stopwatch.reset();
			}
			switch (_flags.front()) {
			case Flag::Information:
				_rect_color = _rect_color_information;
				break;
			case Flag::Error:
				_rect_color = _rect_color_error;
				break;
			}
		}
	}

public:
	void draw() const {
		if (!_texts.empty() && !_flags.empty()) {
			_rect.draw(_rect_color);
			_font(_texts.front()).drawCenter(_font_pos_center);
		}
	}

public:
	void show(String text, Flag flag) {
		_texts.push_back(text);
		_flags.push_back(flag);
	}
};


class UI {
public:
	GUI gui_betting;
	const int32 betting_listbox_rows_max;
	GUI gui_add;
	GUI gui_official;
	SnackBar snackbar;
	Clock clock;

public:
	UI() :
		gui_betting(GUIStyle::Default),
		betting_listbox_rows_max(10),
		gui_add(GUIStyle::Default),
		gui_official(GUIStyle::Default),
		snackbar(),
		clock()
	{
		init();
	}

public:
	void init() {
		GUIStyle gui_style = GUIStyle::Default;
		gui_style.font = Font(11, L"Rounded M+ 1m regular");
		gui_betting = GUI(gui_style);
		gui_add = GUI(gui_style);
		gui_official = GUI(gui_style);
		gui_betting.setPos(0, 0);
		gui_add.setPos(0, Window::Size().y / 2);
		gui_official.setPos(Window::Size().x / 2, 0);
		WidgetStyle widget_style;
		widget_style.font = Font(11, L"Rounded M+ 1m regular");
		gui_betting.setTitle(L"勝馬投票");
		gui_betting.add(L"listbox_arrow_up", GUIButton::Create(L"↓", false, widget_style));
		gui_betting.add(L"listbox_n_pages_max_and_page_current", GUIText::Create(L"--/--", widget_style));
		gui_betting.addln(L"listbox_arrow_down", GUIButton::Create(L"↑", false, widget_style));
		for (int32 row = 0; row < betting_listbox_rows_max; row++) {
			gui_betting.add(Format(L"listbox_index_{}"_fmt, row), GUIText::Create(L"0", widget_style));
			gui_betting.add(Format(L"listbox_delete_{}"_fmt, row), GUIButton::Create(L"削除", false, widget_style));
			gui_betting.addln(Format(L"listbox_row_betting_ticket_{}"_fmt, row), GUIText::Create(L"", widget_style));
		}
		gui_betting.addln(L"listbox_add", GUIButton::Create(L"追加", widget_style));
		gui_betting.add(GUIHorizontalLine::Create(1, widget_style));
		gui_betting.addln(L"init", GUIButton::Create(L"初期化", widget_style));
		gui_add.setTitle(L"追加");
		gui_add.addln(GUIText::Create(L"レース式別", widget_style));
		gui_add.addln(L"race_type", GUIRadioButton::Create({ L"", L"", L"", L"" }, none, true, widget_style));
		gui_add.addln(GUIText::Create(L"馬名", widget_style));
		gui_add.addln(L"horses_name", GUICheckBox::Create({ L"", L"", L"", L"", L"" }, {}, true, widget_style));
		gui_add.addln(GUIText::Create(L"馬名 (馬単, 1着)", widget_style));
		gui_add.addln(L"horse_name_exacta_winner", GUIRadioButton::Create({ L"", L"", L"", L"", L"" }, none, true, widget_style));
		gui_add.addln(GUIText::Create(L"馬名 (馬単, 2着)", widget_style));
		gui_add.addln(L"horse_name_exacta_runner_up", GUIRadioButton::Create({ L"", L"", L"", L"", L"" }, none, true, widget_style));
		gui_add.addln(GUIText::Create(L"発売金", widget_style));
		gui_add.add(GUIText::Create(L"￥", widget_style));
		gui_add.add(L"n_betting_tickets", GUITextField::Create(2, widget_style));
		gui_add.addln(GUIText::Create(L"×100", widget_style));
		gui_add.add(L"confirm", GUIButton::Create(L"確定", widget_style));
		gui_add.addln(L"cancel", GUIButton::Create(L"キャンセル", widget_style));
		gui_official.setTitle(L"着順確定");
		for (int32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			gui_official.addln(GUIText::Create(Format(L"{}着"_fmt, order_of_finish + 1), widget_style));
			gui_official.addln(Format(L"horse_name_{}"_fmt, order_of_finish), GUIRadioButton::Create({ L"", L"", L"", L"", L"" }, none, true, widget_style));
		}
		gui_official.addln(L"confirm", GUIButton::Create(L"確定", widget_style));
		gui_official.add(GUIHorizontalLine::Create(1, widget_style));
		gui_official.addln(L"init", GUIButton::Create(L"初期化", widget_style));
		gui_add.show(false);
	}

public:
	void update() {
		snackbar.update();
		clock.update();
	}

public:
	void draw() const {
		snackbar.draw();
		clock.draw();
	}
};


class Backend {
private:
	Array<BettingTicket> _betting_tickets;
	int32 _n_betting_tickets;
	const int32 _n_betting_tickets_max;
	const int32 _betting_handle_max;
	int32 _listbox_page_current;
	const int32 listbox_n_pages_max;
	Array<HorseName> _results;

public:
	Backend(UI& ui) :
		_betting_tickets(),
		_n_betting_tickets(0),
		_n_betting_tickets_max(100),
		_betting_handle_max(9900),
		_listbox_page_current(0),
		listbox_n_pages_max((_n_betting_tickets_max + ui.betting_listbox_rows_max - 1) / (ui.betting_listbox_rows_max)),
		_results()
	{
		init(ui);
	}

public:
	void init(UI& ui) {
		_init_betting(ui);
		_init_add(ui);
		_init_official(ui);
		if (_write_ini_odds_win_clear()) {
			ui.snackbar.show(L"./runs/odds_win.iniのクリアに成功しました :)", SnackBar::Flag::Information);
		}
		else {
			ui.snackbar.show(L"./runs/odds_win.iniのクリアに失敗しました :(", SnackBar::Flag::Error);
		}
		if (_write_ini_odds_last_clear()) {
			ui.snackbar.show(L"./runs/odds_last.iniのクリアに成功しました :)", SnackBar::Flag::Information);
		}
		else {
			ui.snackbar.show(L"./runs/odds_last.iniのクリアに失敗しました :(", SnackBar::Flag::Error);
		}
	}

public:
	void update(UI& ui) {
		if (ui.gui_betting.button(L"listbox_arrow_up").pushed) {
			_update_betting_listbox_button_arrow_up_and_arrow_down(ui, true);
		}
		if (ui.gui_betting.button(L"listbox_arrow_down").pushed) {
			_update_betting_listbox_button_arrow_up_and_arrow_down(ui, false);
		}
		for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
			if (ui.gui_betting.button(Format(L"listbox_delete_{}"_fmt, row)).pushed) {
				_update_betting_listbox_button_delete(ui, row);
			}
		}
		if (ui.gui_betting.button(L"listbox_add").pushed) {
			_update_betting_listbox_button_add(ui);
		}
		if (ui.gui_betting.button(L"init").pushed) {
			if (MessageBox::Show(L"勝馬投票を初期化しますか?", MessageBoxStyle::YesNo) == MessageBoxCommand::Yes) {
				_init_betting(ui);
				_init_add(ui);
				if (_write_ini_odds_win_clear()) {
					ui.snackbar.show(L"./runs/odds_win.iniのクリアに成功しました :)", SnackBar::Flag::Information);
				}
				else {
					ui.snackbar.show(L"./runs/odds_win.iniのクリアに失敗しました :(", SnackBar::Flag::Error);
				}
			}
		}
		if (ui.gui_add.button(L"confirm").pushed) {
			_update_add_button_confirm(ui);
		}
		if (ui.gui_add.button(L"cancel").pushed) {
			_update_add_button_cancel(ui);
		}
		if (ui.gui_official.button(L"confirm").pushed) {
			_update_official_button_confirm(ui);
		}
		if (ui.gui_official.button(L"init").pushed) {
			if (MessageBox::Show(L"着順確定を初期化しますか?", MessageBoxStyle::YesNo) == MessageBoxCommand::Yes) {
				_init_official(ui);
				if (_write_ini_odds_last_clear()) {
					ui.snackbar.show(L"./runs/odds_last.iniのクリアに成功しました :)", SnackBar::Flag::Information);
				}
				else {
					ui.snackbar.show(L"./runs/odds_last.iniのクリアに失敗しました :(", SnackBar::Flag::Error);
				}
			}
		}
		_foolproof(ui);
	}

private:
	void _init_betting(UI& ui) {
		_betting_tickets.clear();
		_betting_tickets.resize(_n_betting_tickets_max);
		_n_betting_tickets = 0;
		_listbox_page_current = 0;
		ui.gui_betting.text(L"listbox_n_pages_max_and_page_current").text = Format(L"{:>2}/{:>2}"_fmt, _listbox_page_current + 1, listbox_n_pages_max);
		for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
			const int32 index = ui.betting_listbox_rows_max * _listbox_page_current + row;
			ui.gui_betting.text(Format(L"listbox_index_{}"_fmt, row)).text = Format(L"{:>3}"_fmt, index + 1);
			String text;
			if (_betting_tickets.at(index).race_type.type != RaceType::Type::None) {
				text += _betting_tickets.at(index).race_type.to_string();
				text += L", ";
				text += Format(L"￥{}"_fmt, _betting_tickets.at(index).betting_handle);
			}
			ui.gui_betting.text(Format(L"listbox_row_betting_ticket_{}"_fmt, row)).text = text;
		}
	}

private:
	void _init_add(UI& ui) {
		ui.gui_add.radioButton(L"race_type").setName(0, RaceType(RaceType::Type::Win).to_string());
		ui.gui_add.radioButton(L"race_type").setName(1, RaceType(RaceType::Type::Place).to_string());
		ui.gui_add.radioButton(L"race_type").setName(2, RaceType(RaceType::Type::Quinella).to_string());
		ui.gui_add.radioButton(L"race_type").setName(3, RaceType(RaceType::Type::Exacta).to_string());
		ui.gui_add.radioButton(L"race_type").check(0);
		ui.gui_add.checkBox(L"horses_name").setName(0, HorseName(HorseName::Name::A).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_winner").setName(0, HorseName(HorseName::Name::A).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").setName(0, HorseName(HorseName::Name::A).to_string());
		ui.gui_add.checkBox(L"horses_name").setName(1, HorseName(HorseName::Name::B).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_winner").setName(1, HorseName(HorseName::Name::B).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").setName(1, HorseName(HorseName::Name::B).to_string());
		ui.gui_add.checkBox(L"horses_name").setName(2, HorseName(HorseName::Name::C).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_winner").setName(2, HorseName(HorseName::Name::C).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").setName(2, HorseName(HorseName::Name::C).to_string());
		ui.gui_add.checkBox(L"horses_name").setName(3, HorseName(HorseName::Name::D).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_winner").setName(3, HorseName(HorseName::Name::D).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").setName(3, HorseName(HorseName::Name::D).to_string());
		ui.gui_add.checkBox(L"horses_name").setName(4, HorseName(HorseName::Name::E).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_winner").setName(4, HorseName(HorseName::Name::E).to_string());
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").setName(4, HorseName(HorseName::Name::E).to_string());
		for (uint32 index = 0; index < ui.gui_add.checkBox(L"horses_name").num_items; index++) {
			ui.gui_add.checkBox(L"horses_name").check(index, false);
		}
		ui.gui_add.radioButton(L"horse_name_exacta_winner").check(none);
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").check(none);
		ui.gui_add.textField(L"n_betting_tickets").setText(L"1");
	}

private:
	void _init_official(UI& ui) {
		_results.clear();
		_results.resize(5);
		for (uint32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).setName(0, HorseName(HorseName::Name::A).to_string());
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).setName(1, HorseName(HorseName::Name::B).to_string());
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).setName(2, HorseName(HorseName::Name::C).to_string());
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).setName(3, HorseName(HorseName::Name::D).to_string());
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).setName(4, HorseName(HorseName::Name::E).to_string());
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).check(none);
		}
	}

private:
	void _update_betting_listbox_button_arrow_up_and_arrow_down(UI& ui, const bool& is_up) {
		if (is_up) {
			_listbox_page_current--;
		}
		else {
			_listbox_page_current++;
		}
		ui.gui_betting.text(L"listbox_n_pages_max_and_page_current").text = Format(L"{:>2}/{:>2}"_fmt, _listbox_page_current + 1, listbox_n_pages_max);
		for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
			const int32 index = ui.betting_listbox_rows_max * _listbox_page_current + row;
			ui.gui_betting.text(Format(L"listbox_index_{}"_fmt, row)).text = Format(L"{:>3}"_fmt, index + 1);
			String text;
			if (_betting_tickets.at(index).race_type.type != RaceType::Type::None) {
				text += _betting_tickets.at(index).race_type.to_string();
				text += L", ";
				text += Format(L"￥{}"_fmt, _betting_tickets.at(index).betting_handle);
			}
			ui.gui_betting.text(Format(L"listbox_row_betting_ticket_{}"_fmt, row)).text = text;
		}
	}

private:
	void _update_betting_listbox_button_delete(UI& ui, const int32& row) {
		const int32 index = ui.betting_listbox_rows_max * _listbox_page_current + row;
		_betting_tickets.at(index).betting_handle = 0;
		String text;
		if (_betting_tickets.at(index).race_type.type != RaceType::Type::None) {
			text += _betting_tickets.at(index).race_type.to_string();
			text += L", ";
			text += Format(L"￥{}"_fmt, _betting_tickets.at(index).betting_handle);
		}
		ui.gui_betting.text(Format(L"listbox_row_betting_ticket_{}"_fmt, row)).text = text;
		if (_write_ini_odds_win(_odds_win(ui))) {
			ui.snackbar.show(L"./runs/odds_win.iniの書き込みに成功しました :)", SnackBar::Flag::Information);
		}
		else {
			ui.snackbar.show(L"./runs/odds_win.iniの書き込みに失敗しました :(", SnackBar::Flag::Error);
		}
		ui.gui_add.show(false);
	}

private:
	void _update_betting_listbox_button_add(UI& ui) {
		_init_add(ui);
		ui.gui_add.show(true);
	}

private:
	void _update_add_button_confirm(UI& ui) {
		switch (ui.gui_add.radioButton(L"race_type").checkedItem.value()) {
		case 0:
			_betting_tickets.at(_n_betting_tickets).race_type.type = RaceType::Type::Win;
			break;
		case 1:
			_betting_tickets.at(_n_betting_tickets).race_type.type = RaceType::Type::Place;
			break;
		case 2:
			_betting_tickets.at(_n_betting_tickets).race_type.type = RaceType::Type::Quinella;
			break;
		case 3:
			_betting_tickets.at(_n_betting_tickets).race_type.type = RaceType::Type::Exacta;
			break;
		}
		for (int32 index = 0; index < ui.gui_add.checkBox(L"horses_name").checkedItems.size(); index++) {
			HorseName horse_name;
			switch (ui.gui_add.checkBox(L"horses_name").checkedItems.at(index)) {
			case 0:
				horse_name.name = HorseName::Name::A;
				break;
			case 1:
				horse_name.name = HorseName::Name::B;
				break;
			case 2:
				horse_name.name = HorseName::Name::C;
				break;
			case 3:
				horse_name.name = HorseName::Name::D;
				break;
			case 4:
				horse_name.name = HorseName::Name::E;
				break;
			}
			_betting_tickets.at(_n_betting_tickets).horses_name.push_back(horse_name);
		}
		if (ui.gui_add.radioButton(L"horse_name_exacta_winner").checkedItem.has_value()) {
			switch (ui.gui_add.radioButton(L"horse_name_exacta_winner").checkedItem.value()) {
			case 0:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_winner.name = HorseName::Name::A;
				break;
			case 1:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_winner.name = HorseName::Name::B;
				break;
			case 2:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_winner.name = HorseName::Name::C;
				break;
			case 3:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_winner.name = HorseName::Name::D;
				break;
			case 4:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_winner.name = HorseName::Name::E;
				break;
			}
		}
		if (ui.gui_add.radioButton(L"horse_name_exacta_runner_up").checkedItem.has_value()) {
			switch (ui.gui_add.radioButton(L"horse_name_exacta_runner_up").checkedItem.value()) {
			case 0:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_runner_up.name = HorseName::Name::A;
				break;
			case 1:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_runner_up.name = HorseName::Name::B;
				break;
			case 2:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_runner_up.name = HorseName::Name::C;
				break;
			case 3:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_runner_up.name = HorseName::Name::D;
				break;
			case 4:
				_betting_tickets.at(_n_betting_tickets).horse_name_exacta_runner_up.name = HorseName::Name::E;
				break;
			}
		}
		_betting_tickets.at(_n_betting_tickets).betting_handle = 100 * Parse<int32>(ui.gui_add.textField(L"n_betting_tickets").text);
		const int32 row = _n_betting_tickets - ui.betting_listbox_rows_max * _listbox_page_current;
		if (0 <= row && row < ui.betting_listbox_rows_max) {
			String text;
			if (_betting_tickets.at(_n_betting_tickets).race_type.type != RaceType::Type::None) {
				text += _betting_tickets.at(_n_betting_tickets).race_type.to_string();
				text += L", ";
				text += Format(L"￥{}"_fmt, _betting_tickets.at(_n_betting_tickets).betting_handle);
			}
			ui.gui_betting.text(Format(L"listbox_row_betting_ticket_{}"_fmt, row)).text = text;
		}
		_n_betting_tickets++;
		if (_write_ini_odds_win(_odds_win(ui))) {
			ui.snackbar.show(L"./runs/odds_win.iniの書き込みに成功しました :)", SnackBar::Flag::Information);
		}
		else {
			ui.snackbar.show(L"./runs/odds_win.iniの書き込みに失敗しました :(", SnackBar::Flag::Error);
		}
		if (_foolproof_official(ui)) {
			if (_write_ini_odds_last(_odds_last(ui))) {
				ui.snackbar.show(L"./runs/odds_last.iniの書き込みの成功しました :)", SnackBar::Flag::Information);
			}
			else {
				ui.snackbar.show(L"./runs/odds_last.iniの書き込みに失敗しました :(", SnackBar::Flag::Error);
			}
		}
		ui.gui_add.show(false);
	}

private:
	void _update_add_button_cancel(UI& ui) {
		ui.gui_add.show(false);
	}

private:
	void _update_official_button_confirm(UI& ui) {
		for (uint32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			switch (ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).checkedItem.value()) {
			case 0:
				_results.at(order_of_finish).name = HorseName::Name::A;
				break;
			case 1:
				_results.at(order_of_finish).name = HorseName::Name::B;
				break;
			case 2:
				_results.at(order_of_finish).name = HorseName::Name::C;
				break;
			case 3:
				_results.at(order_of_finish).name = HorseName::Name::D;
				break;
			case 4:
				_results.at(order_of_finish).name = HorseName::Name::E;
				break;
			}
		}
		if (_write_ini_odds_last(_odds_last(ui))) {
			ui.snackbar.show(L"./runs/odds_last.iniの書き込みに成功しました :)", SnackBar::Flag::Information);
		}
		else {
			ui.snackbar.show(L"./runs/odds_last.iniの書き込みに失敗しました :(", SnackBar::Flag::Error);
		}
	}

private:
	void _foolproof(UI& ui) {
		ui.gui_betting.button(L"listbox_arrow_up").enabled = true;
		ui.gui_betting.button(L"listbox_arrow_down").enabled = true;
		for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
			ui.gui_betting.button(Format(L"listbox_delete_{}"_fmt, row)).enabled = true;
		}
		ui.gui_betting.button(L"listbox_add").enabled = true;
		ui.gui_betting.button(L"init").enabled = true;
		ui.gui_add.radioButton(L"race_type").enabled = true;
		ui.gui_add.checkBox(L"horses_name").enabled = true;
		ui.gui_add.radioButton(L"horse_name_exacta_winner").enabled = true;
		ui.gui_add.radioButton(L"horse_name_exacta_runner_up").enabled = true;
		ui.gui_add.textField(L"n_betting_tickets").enabled = true;
		ui.gui_add.button(L"confirm").enabled = true;
		ui.gui_add.button(L"cancel").enabled = true;
		if (_listbox_page_current == 0) {
			ui.gui_betting.button(L"listbox_arrow_up").enabled = false;
		}
		if (_listbox_page_current == listbox_n_pages_max - 1) {
			ui.gui_betting.button(L"listbox_arrow_down").enabled = false;
		}
		for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
			const int32 index = ui.betting_listbox_rows_max * _listbox_page_current + row;
			if (_betting_tickets.at(index).race_type.type == RaceType::Type::None || _betting_tickets.at(index).betting_handle == 0) {
				ui.gui_betting.button(Format(L"listbox_delete_{}"_fmt, row)).enabled = false;
			}
		}
		if (_n_betting_tickets == _n_betting_tickets_max - 1) {
			ui.gui_betting.button(L"listbox_add").enabled = false;
		}
		switch (ui.gui_add.radioButton(L"race_type").checkedItem.value()) {
		case 0:
			ui.gui_add.radioButton(L"horse_name_exacta_winner").enabled = false;
			ui.gui_add.radioButton(L"horse_name_exacta_runner_up").enabled = false;
			if (ui.gui_add.checkBox(L"horses_name").checkedItems.size() != 1) {
				ui.gui_add.button(L"confirm").enabled = false;
			}
			break;
		case 1:
			ui.gui_add.radioButton(L"horse_name_exacta_winner").enabled = false;
			ui.gui_add.radioButton(L"horse_name_exacta_runner_up").enabled = false;
			if (ui.gui_add.checkBox(L"horses_name").checkedItems.size() != 1) {
				ui.gui_add.button(L"confirm").enabled = false;
			}
			break;
		case 2:
			ui.gui_add.radioButton(L"horse_name_exacta_winner").enabled = false;
			ui.gui_add.radioButton(L"horse_name_exacta_runner_up").enabled = false;
			if (ui.gui_add.checkBox(L"horses_name").checkedItems.size() != 2) {
				ui.gui_add.button(L"confirm").enabled = false;
			}
			break;
		case 3:
			ui.gui_add.checkBox(L"horses_name").enabled = false;
			if (!(ui.gui_add.radioButton(L"horse_name_exacta_winner").checkedItem.has_value() && ui.gui_add.radioButton(L"horse_name_exacta_runner_up").checkedItem.has_value())) {
				ui.gui_add.button(L"confirm").enabled = false;
			}
			else if (ui.gui_add.radioButton(L"horse_name_exacta_winner").checkedItem.value() == ui.gui_add.radioButton(L"horse_name_exacta_runner_up").checkedItem.value()) {
				ui.gui_add.button(L"confirm").enabled = false;
			}
			break;
		}
		if (!ParseOpt<int32>(ui.gui_add.textField(L"n_betting_tickets").text)) {
			ui.gui_add.button(L"confirm").enabled = false;
		}
		else if (!(1 <= Parse<int32>(ui.gui_add.textField(L"n_betting_tickets").text) && Parse<int32>(ui.gui_add.textField(L"n_betting_tickets").text) <= _betting_handle_max / 100)) {
			ui.gui_add.button(L"confirm").enabled = false;
		}
		if (ui.gui_add.style.visible) {
			ui.gui_betting.button(L"listbox_arrow_up").enabled = false;
			ui.gui_betting.button(L"listbox_arrow_down").enabled = false;
			for (int32 row = 0; row < ui.betting_listbox_rows_max; row++) {
				ui.gui_betting.button(Format(L"listbox_delete_{}"_fmt, row)).enabled = false;
			}
			ui.gui_betting.button(L"listbox_add").enabled = false;
			ui.gui_betting.button(L"init").enabled = false;
			for (int32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
				ui.gui_official.radioButton(Format(L"listbox_horse_name_{}"_fmt, order_of_finish)).enabled = false;
			}
		}
		for (int32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).enabled = true;
		}
		ui.gui_official.button(L"confirm").enabled = true;
		ui.gui_official.button(L"init").enabled = true;
		std::set<int32> checked_items;
		for (int32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			if (ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).checkedItem.has_value()) {
				checked_items.insert(static_cast<int32>(ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).checkedItem.value()));
			}
		}
		if (checked_items.size() != 5) {
			ui.gui_official.button(L"confirm").enabled = false;
		}
	}

private:
	bool _foolproof_official(UI& ui) {
		std::set<int32> checked_items;
		for (int32 order_of_finish = 0; order_of_finish < 5; order_of_finish++) {
			if (ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).checkedItem.has_value()) {
				checked_items.insert(static_cast<int32>(ui.gui_official.radioButton(Format(L"horse_name_{}"_fmt, order_of_finish)).checkedItem.value()));
			}
		}
		if (checked_items.size() != 5) {
			return false;
		}
		return true;
	}

private:
	Array<OddsWin> _odds_win(UI& ui) {
		Array<OddsWin> odds_win(5);
		odds_win.at(0).result.name = HorseName::Name::A;
		odds_win.at(1).result.name = HorseName::Name::B;
		odds_win.at(2).result.name = HorseName::Name::C;
		odds_win.at(3).result.name = HorseName::Name::D;
		odds_win.at(4).result.name = HorseName::Name::E;
		double S = 0.0;
		Array<double> S_i(5);
		for (int32 index = 0; index < _n_betting_tickets; index++) {
			if (_betting_tickets.at(index).race_type.type == RaceType::Type::Win) {
				S += _betting_tickets.at(index).betting_handle;
				switch (_betting_tickets.at(index).horses_name.front().name) {
				case HorseName::Name::A:
					S_i.at(0) += _betting_tickets.at(index).betting_handle;
					break;
				case HorseName::Name::B:
					S_i.at(1) += _betting_tickets.at(index).betting_handle;
					break;
				case HorseName::Name::C:
					S_i.at(2) += _betting_tickets.at(index).betting_handle;
					break;
				case HorseName::Name::D:
					S_i.at(3) += _betting_tickets.at(index).betting_handle;
					break;
				case HorseName::Name::E:
					S_i.at(4) += _betting_tickets.at(index).betting_handle;
					break;
				}
			}
		}
		for (int32 index = 0; index < 5; index++) {
			if (S_i.at(index) != 0.0) {
				odds_win.at(index).odds = S / S_i.at(index);
			}
			else {
				odds_win.at(index).odds = 1.0;
			}
			odds_win.at(index).odds = Max(odds_win.at(index).odds, 1.0);
		}
		return odds_win;
	}

private:
	bool _write_ini_odds_win(Array<OddsWin> odds_win) {
		INIWriter ini_writer(L"./runs/odds_win.ini");
		if (!ini_writer) {
			return false;
		}
		for (int32 index = 0; index < 5; index++) {
			ini_writer.write(L"OddsWin_HorseName", Format(L"{}"_fmt, index), odds_win.at(index).result.to_string());
		}
		for (int32 index = 0; index < 5; index++) {
			ini_writer.write(L"OddsWin_Odds", Format(L"{}"_fmt, index), odds_win.at(index).odds);
		}
		return true;
	}

private:
	bool _write_ini_odds_win_clear() {
		INIWriter ini_writer(L"./runs/odds_win.ini");
		if (!ini_writer) {
			return false;
		}
		ini_writer.clear();
		return true;
	}

private:
	OddsLast _odds_last(UI& ui) {
		OddsLast odds_last;
		odds_last.odds_win.result.name = _results.front().name;
		for (int32 order_of_finish = 0; order_of_finish < 3; order_of_finish++) {
			odds_last.odds_place.results.at(order_of_finish).name = _results.at(order_of_finish).name;
		}
		for (int32 order_of_finish = 0; order_of_finish < 2; order_of_finish++) {
			odds_last.odds_quinella.results.at(order_of_finish).name = _results.at(order_of_finish).name;
		}
		for (int32 order_of_finish = 0; order_of_finish < 2; order_of_finish++) {
			odds_last.odds_exacta.results.at(order_of_finish).name = _results.at(order_of_finish).name;
		}
		Array<Array<double>> W(4);
		Array<double> D(4);
		Array<double> P(4);
		W.at(0).resize(1);
		W.at(1).resize(3);
		W.at(2).resize(1);
		W.at(3).resize(1);
		for (int32 index = 0; index < _n_betting_tickets; index++) {
			switch (_betting_tickets.at(index).race_type.type) {
			case RaceType::Type::Win:
				if (_betting_tickets.at(index).horses_name.front().name == _results.front().name) {
					W.at(0).front() += _betting_tickets.at(index).betting_handle;
				}
				else {
					D.at(0) += _betting_tickets.at(index).betting_handle;
				}
				break;
			case RaceType::Type::Place:
				if (_betting_tickets.at(index).horses_name.front().name == _results.at(0).name) {
					W.at(1).at(0) += _betting_tickets.at(index).betting_handle;
				}
				else if (_betting_tickets.at(index).horses_name.front().name == _results.at(1).name) {
					W.at(1).at(1) += _betting_tickets.at(index).betting_handle;
				}
				else if (_betting_tickets.at(index).horses_name.front().name == _results.at(2).name) {
					W.at(1).at(2) += _betting_tickets.at(index).betting_handle;
				}
				else {
					D.at(1) += _betting_tickets.at(index).betting_handle;
				}
				break;
			case RaceType::Type::Quinella:
				if (_betting_tickets.at(index).horses_name.at(0).name == _results.at(0).name && _betting_tickets.at(index).horses_name.at(1).name == _results.at(1).name ||
					_betting_tickets.at(index).horses_name.at(0).name == _results.at(1).name && _betting_tickets.at(index).horses_name.at(1).name == _results.at(0).name) {
					W.at(2).front() += _betting_tickets.at(index).betting_handle;
				}
				else {
					D.at(2) += _betting_tickets.at(index).betting_handle;
				}
				break;
			case RaceType::Type::Exacta:
				if (_betting_tickets.at(index).horse_name_exacta_winner.name == _results.at(0).name && _betting_tickets.at(index).horse_name_exacta_runner_up.name == _results.at(1).name) {
					W.at(3).front() += _betting_tickets.at(index).betting_handle;
				}
				else {
					D.at(3) += _betting_tickets.at(index).betting_handle;
				}
				break;
			}
		}
		P.at(0) = 1.0;
		P.at(1) = 3.0;
		P.at(2) = 2.0;
		P.at(3) = 2.0;
		if (W.at(0).front() != 0.0) {
			odds_last.odds_win.odds = (W.at(0).front() + D.at(0) / P.at(0)) / W.at(0).front();
		}
		else {
			odds_last.odds_win.odds = 1.0;
		}
		odds_last.odds_win.odds = Max(odds_last.odds_win.odds, 1.0);
		for (int32 order_of_finish = 0; order_of_finish < 3; order_of_finish++) {
			if (W.at(1).at(order_of_finish) != 0.0) {
				odds_last.odds_place.odds.at(order_of_finish) = (W.at(1).at(order_of_finish) + D.at(1) / P.at(1)) / W.at(1).at(order_of_finish);
			}
			else {
				odds_last.odds_place.odds.at(order_of_finish) = 1.0;
			}
			odds_last.odds_place.odds.at(order_of_finish) = Max(odds_last.odds_place.odds.at(order_of_finish), 1.0);
		}
		if (W.at(2).front() != 0.0) {
			odds_last.odds_quinella.odds = (W.at(2).front() + D.at(2) / P.at(2)) / W.at(2).front();
		}
		else {
			odds_last.odds_quinella.odds = 1.0;
		}
		odds_last.odds_quinella.odds = Max(odds_last.odds_quinella.odds, 1.0);
		if (W.at(3).front() != 0.0) {
			odds_last.odds_exacta.odds = (W.at(3).front() + D.at(3) / P.at(3)) / W.at(3).front();
		}
		else {
			odds_last.odds_exacta.odds = 1.0;
		}
		odds_last.odds_exacta.odds = Max(odds_last.odds_exacta.odds, 1.0);
		return odds_last;
	}

private:
	bool _write_ini_odds_last(OddsLast odds_last) {
		INIWriter ini_writer(L"./runs/odds_last.ini");
		if (!ini_writer) {
			return false;
		}
		ini_writer.write(L"OddsWin_HorseName", Format(L"{}"_fmt, 0), odds_last.odds_win.result.to_string());
		ini_writer.write(L"OddsWin_Odds", Format(L"{}"_fmt, 0), odds_last.odds_win.odds);
		for (int32 order_of_finish = 0; order_of_finish < 3; order_of_finish++) {
			ini_writer.write(L"OddsPlace_HorseName", Format(L"{}"_fmt, order_of_finish), odds_last.odds_place.results.at(order_of_finish).to_string());
			ini_writer.write(L"OddsPlace_Odds", Format(L"{}"_fmt, order_of_finish), odds_last.odds_place.odds.at(order_of_finish));
		}
		for (int32 order_of_finish = 0; order_of_finish < 2; order_of_finish++) {
			ini_writer.write(L"OddsQuinella_HorseName", Format(L"{}"_fmt, order_of_finish), odds_last.odds_quinella.results.at(order_of_finish).to_string());
		}
		ini_writer.write(L"OddsQuinella_Odds", Format(L"{}"_fmt, 0), odds_last.odds_quinella.odds);
		for (int32 order_of_finish = 0; order_of_finish < 2; order_of_finish++) {
			ini_writer.write(L"OddsExacta_HorseName", Format(L"{}"_fmt, order_of_finish), odds_last.odds_exacta.results.at(order_of_finish).to_string());
		}
		ini_writer.write(L"OddsExacta_Odds", Format(L"{}"_fmt, 0), odds_last.odds_exacta.odds);
		return true;
	}

private:
	bool _write_ini_odds_last_clear() {
		INIWriter ini_writer(L"./runs/odds_last.ini");
		if (!ini_writer) {
			return false;
		}
		ini_writer.clear();
		return true;
	}
};


void Main() {
	Window::SetTitle(L"5I Horse Race Manager");
	Graphics::SetBackground(Color(51));
	ScreenCapture::EnableScreenshot(false);
	Window::SetFullscreen(true, Size(1920, 1080));
	FontManager::Register(L"/64608");
	UI ui;
	Backend backend(ui);
	while (System::Update()) {
		backend.update(ui);
		ui.update();
		ui.draw();
	}
}
