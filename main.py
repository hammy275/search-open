# This is all contained in one Python script to maximize portability
from xdg.DesktopEntry import DesktopEntry
from xdg.Exceptions import ParsingError, DuplicateGroupError, DuplicateKeyError
from os import listdir, execlp
from os.path import expanduser, isfile, isdir, join
from dataclasses import dataclass, field
from heapq import heappush
from shutil import which
from gi import require_version

require_version('Gtk', '3.0')

from gi.repository import Gtk, GdkPixbuf
# TODO: Figure out why tarstall entries aren't showing

@dataclass(order=True)
class PriorityDesktopEntry:
    priority: int
    entry: DesktopEntry = field(compare=False)

    def __init__(self, entry: DesktopEntry, priority: int):
        self.entry = entry
        self.priority = priority


class Main:
    def __init__(self):
        self.desktops = []
        self.get_desktops()

        self.builder = Gtk.Builder()
        self.builder.add_from_file("view.glade")

        self.window = self.builder.get_object("main")
        self.window.set_decorated(False)
        self.window.connect("focus-out-event", Gtk.main_quit)
        self.window.connect("destroy", Gtk.main_quit)  # Quit when focus is lost or on X pressed

        self.search_bar = self.builder.get_object("input")
        self.search_bar.connect("changed", self.on_search_changed)
        self.entries = []

        self.result_images = []
        self.result_labels = []
        for i in range(1, 6):
            self.result_labels.append(self.builder.get_object("result-text-" + str(i)))
            self.result_images.append(self.builder.get_object("result-image-" + str(i)))

        self.window.show_all()
        Gtk.main()

    def on_search_changed(self, search_widget):
        search_term = search_widget.get_text()
        self.search(search_term, self.desktops)
        for i in range(5):
            try:
                entry = self.entries[i].entry
            except IndexError:
                self.result_labels[i].hide()
                self.result_images[i].hide()
                continue
            self.result_labels[i].show()
            self.result_images[i].show()
            self.result_labels[i].set_text(entry.getName())
            icon = entry.getIcon()
            if isfile(icon):
                pixbuf = GdkPixbuf.Pixbuf.new_from_file_at_scale(icon, 48, 48, False)
                self.result_images[i].set_from_pixbuf(pixbuf)
            else:
                self.result_images[i].set_from_icon_name(entry.getIcon(), 6)
                #self.result_images[i].set_pixel_size(32)

    def get_matching_score(self, query: str, other: str):
        """Score Between Two Strings.

        Gets a score between two strings for how much they match. The LOWER, the better.

        Args:
            query: Query string
            other: The other input string.

        Returns:
            The matching score. Returns 0 if not a match in any way.

        """
        # Stupid basic for now.
        if other.strip() == "":
            return 0
        if query in other or other in query:
            return -1
        return 0

    def _search(self, query: str, entries, search_type: str):
        """Search Entries with Type

        Search through the entries with the given type. Case-sensitive!

        Supported types:
            - name
            - comment

        Args:
            query (str): Query to search for
            entries (DesktopEntry[]): List of entries to search through
            search_type: Type of search to make

        Returns:
            PriorityDesktopEntry[]: List of entries matching query. Ordered from best (index 0) to worst (index(len(entries)).

        """
        to_ret = []
        for e in entries:
            to_check: str = e.getName() if search_type == "name" else e.getComment() if search_type == "comment" else ""
            priority = self.get_matching_score(query, to_check.lower())
            if priority < 0:
                heappush(to_ret, PriorityDesktopEntry(e, priority))
        return to_ret

    def search(self, query: str, entries):
        """Search Entries for Query

        Sets self.entries to a PriorityDesktopEntry[] representing the search results.

        Args:
            query (str): Query to search with
            entries (DesktopEntry[]): List of desktop entries to search through
        """
        query = query.lower()
        name_res = self._search(query, entries, "name")
        comment_res = self._search(query, entries, "comment")
        self.entries = name_res + comment_res

    def get_desktops(self):
        """Update Desktop Objects.

        Gets all the desktop entries available on the system. Not case-sensitive!

        Sets self.desktops to DesktopObject[] representing all available .desktop files


        """
        entries = []
        desktop_root_folders = ["/usr/share/applications/", expanduser("~/.local/share/applications/"),
                                "/usr/local/share/applications"]
        desktop_folders = []
        for f in desktop_root_folders:
            desktop_folders = desktop_folders + self.get_subdirs(f)
        for folder in desktop_folders:
            try:
                for entry_str in listdir(folder):
                    if not entry_str.endswith(".desktop"):
                        continue
                    try:
                        entry = DesktopEntry(filename=folder + entry_str)
                        if entry.getType() == "Application":
                            entry.desktop_file_name = entry_str[:-8]
                            entries.append(entry)
                    except (ParsingError, DuplicateGroupError, DuplicateKeyError):
                        continue
            except (FileNotFoundError, NotADirectoryError):
                continue
        self.desktops = entries

    def get_subdirs(self, folder):
        """Get All Subdirs.

        Get all subdirectories of a folder plus that folder.

        Args:
            folder: Path to get subdirs of

        Returns:
            str[]: List of all folders inside of folder along with folder
        """
        folders = [folder]
        try:
            for f in listdir(folder):
                if isdir(join(folder, f)):
                    folders = folders + self.get_subdirs(join(folder, f))
        except FileNotFoundError:
            return []
        return folders


if __name__ == "__main__":
    if not which("gtk-launch"):
        raise FileNotFoundError("gtk-launch not found!")
    Main()
    # execlp("gtk-launch", "gtk-launch", search_res[to_open - 1].entry.desktop_file_name)
